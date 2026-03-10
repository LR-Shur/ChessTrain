#include <QPainter>
#include "BoardWidget.h"
#include <QMouseEvent>
#include <cmath>

// 构造函数
BoardWidget::BoardWidget(ChessGame *game, QWidget *parent, ChessAi *chessAi)
    : QWidget(parent) // 调用基类 QWidget 的构造函数
      ,
      m_game(game) // 保存传入的游戏对象指针
      ,
      m_ai(chessAi)

{
    setMinimumSize(500, 500); // 设置控件的最小尺寸，防止窗口缩得太小
}

// 更换游戏对象
void BoardWidget::setGame(ChessGame *game)
{
    m_game = game;
    update(); // 请求重绘棋盘
}

void BoardWidget::restart()
{
    if (!m_game)
        return;

    // 1. 重置所有状态（避免残留状态导致逻辑错误）
    m_clickEnabled = true;
    m_isAiTurn = false;
    // 2. 清空棋盘
    m_game->initBoard();

    // 3. 重绘棋盘
    update();
    // 4. 判断当前玩家是否是AI，若是则主动触发AI落子
    if (isCurrentPlayerAi())
    {
        m_isAiTurn = true;
        m_clickEnabled = false; // AI回合禁用玩家点击
        onAiMove();             // 触发AI落子
    }
}

// 计算当前控件大小下最佳的格子尺寸和边距
void BoardWidget::calculateBoardParams(int &cellSize, int &margin) const
{
    int boardSize = ChessGame::getBoardSize(); // 15

    // 可用区域：控件宽度和高度（取较小值以保证棋盘为正方形）
    int available = std::min(width(), height());

    // 我们希望棋盘占满整个控件，但留一点边距（比如 20 像素）
    margin = 20;
    int boardPixels = available - 2 * margin; // 棋盘实际占用的像素宽度

    // 格子大小 = 棋盘像素宽度 / (boardSize - 1)
    // 因为 boardSize 条线，有 boardSize-1 个格子间隔
    cellSize = boardPixels / (boardSize - 1);

    // 确保格子至少为 1 像素
    if (cellSize < 1)
        cellSize = 1;

    // 重新计算边距，使棋盘居中（可选）
    // 让 margin 稍微调整，使棋盘在可用区域内居中
    int totalBoardPixels = (boardSize - 1) * cellSize;
    margin = (available - totalBoardPixels) / 2;
}

// 绘制事件
void BoardWidget::paintEvent(QPaintEvent *event)
{

    // 先初始化 painter，再判断 m_game（避免 painter 状态异常）
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿，让线条更平滑

    // 如果没有游戏对象，就不绘制
    if (!m_game)
        return;

    // 获取棋盘大小
    int cellSize, margin;
    calculateBoardParams(cellSize, margin);
    int boardSize = ChessGame::getBoardSize();
    int boardWidth = (boardSize - 1) * cellSize;

    // 绘制背景
    painter.fillRect(rect(), QColor(210, 180, 140)); // 木色

    // 绘制网格线
    painter.setPen(Qt::black);
    for (int i = 0; i < boardSize; ++i)
    {
        int x = margin + i * cellSize;
        int y = margin + i * cellSize;
        // 竖线
        painter.drawLine(x, margin, x, margin + boardWidth);
        // heng
        painter.drawLine(margin, y, margin + boardWidth, y);
    }

    // 绘制天元（中心点），只是一个装饰
    int center = margin + (boardSize / 2) * cellSize;
    painter.setBrush(Qt::black);
    painter.drawEllipse(center - 3, center - 3, 6, 6);

    // 绘制棋子
    for (int row = 0; row < boardSize; ++row)
    {
        for (int col = 0; col < boardSize; ++col)
        {
            // 获取该位置的棋子颜色，0 表示空
            int piece = m_game->getPieceAt(row, col);
            if (piece == 0 || piece == -1)
                continue;

            // 计算棋子中心坐标
            int x = margin + col * cellSize;
            int y = margin + row * cellSize;

            // 根据颜色设置画刷
            if (piece == 1)
                painter.setBrush(Qt::black);
            else if (piece == 2)
                painter.setBrush(Qt::white);

            painter.setPen(Qt::darkGray); // 棋子边缘颜色
            painter.drawEllipse(x - cellSize / 2, y - cellSize / 2, cellSize, cellSize);
        }
    }
}

// 鼠标按下事件
void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (!m_clickEnabled || m_isAiTurn || !m_game)
        return;
    if (event->button() != Qt::LeftButton)
        return; // 只处理左键

    // 获取点击位置的像素坐标
    int x = event->pos().x();
    int y = event->pos().y();

    int cellSize, margin;
    calculateBoardParams(cellSize, margin);

    // 将像素坐标转换为棋盘行列
    // 公式： (像素坐标 - 边距) / 格子大小，然后四舍五入到最近的整数
    int col = std::round((x - margin) / (double)cellSize);
    int row = std::round((y - margin) / (double)cellSize);

    // 检查是否在棋盘范围内
    if (row < 0 || row >= ChessGame::getBoardSize() ||
        col < 0 || col >= ChessGame::getBoardSize())
        return;

    // 尝试落子
    bool success = m_game->placePiece(row, col);
    if (!success)
        return; // 落子失败（如位置已有棋子），直接返回

    // 落子成功，重绘棋盘
    update();

    // 发射信号：通知主窗口下一个玩家是谁（因为 placePiece 已经切换了玩家）
    emit piecePlaced(m_game->getCurrentPlayer());

    // 检查游戏是否结束
    checkGameOver();

    // 7. 判断是否轮到AI落子
    if (isCurrentPlayerAi() && !m_game->checkWinGlobal())
    {
        m_isAiTurn = true;      // 标记AI回合
        m_clickEnabled = false; // 禁用玩家点击
        onAiMove();
    }
}

void BoardWidget::onAiMove()
{
    cout << "onaimove";
    if (!m_game || !m_ai)
        return;

    // 将AI计算放到后台线程
    QtConcurrent::run([this]()
                      {
        // 后台计算最佳落点
        std::vector<int> aiMove = m_ai->getBestMove(m_game->getAiColor(), 4, 2, 10000);
        // 计算完成后切回主线程更新UI
        QMetaObject::invokeMethod(this, "onAiMoveFinished",
                                  Qt::QueuedConnection,
                                  Q_ARG(std::vector<int>, aiMove)); });
}

void BoardWidget::checkGameOver()
{
    int winner = m_game->checkWinGlobal();
    if (winner != 0)
    {
        m_clickEnabled = false; // 游戏结束禁用点击
        emit gameOver(winner);  // 通知主窗口
    }
    else if (m_game->isBoardFull())
    {
        m_clickEnabled = false;
        emit gameOver(0); // 平局
    }
}

void BoardWidget::onAiMoveFinished(std::vector<int> aiMove)
{
    if (!m_game || aiMove.size() < 2)
    {
        m_isAiTurn = false;
        m_clickEnabled = true;
        return;
    }

    int aiRow = aiMove[0];
    int aiCol = aiMove[1];
    // AI落子
    if (m_game->placePiece(aiRow, aiCol))
    {
        update();        // 重绘棋盘
        checkGameOver(); // 检查游戏结束
    }

    // 恢复玩家操作
    m_isAiTurn = false;
    m_clickEnabled = true;
}

bool BoardWidget::isCurrentPlayerAi() const
{
    if (!m_game)
        return false;
    // 核心：当前玩家 == AI颜色 → 是AI回合
    return m_game->getCurrentPlayer() == m_game->getAiColor();
}
