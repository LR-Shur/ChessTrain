#include <QPainter>
#include "BoardWidget.h"
#include <QMouseEvent>
#include <cmath>

// 构造函数
BoardWidget::BoardWidget(ChessGame *game, QWidget *parent)
    : QWidget(parent) // 调用基类 QWidget 的构造函数
      ,
      m_game(game) // 保存传入的游戏对象指针

{
    setMinimumSize(500, 500); // 设置控件的最小尺寸，防止窗口缩得太小
}

// 更换游戏对象
void BoardWidget::setGame(ChessGame *game)
{
    m_game = game;
    update(); // 请求重绘棋盘
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
    // 如果没有游戏对象，就不绘制
    if (!m_game)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿，让线条更平滑

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
    if (!m_game)
        return; // 没有游戏对象，忽略点击
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
    int winner = m_game->checkWinGlobal();
    if (winner != 0)
    {
        emit gameOver(winner); // 发射游戏结束信号
    }
    else if (m_game->isBoardFull())
    {
        emit gameOver(0); // 棋盘满了，平局
    }
}