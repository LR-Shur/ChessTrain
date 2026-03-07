#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BoardWidget.h"
#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_game(new ChessGame()), m_boardWidget(nullptr)
{
        ui->setupUi(this);
        // 创建棋盘
        m_boardWidget = new BoardWidget(m_game, this);
        std::srand(static_cast<unsigned>(std::time(nullptr))); // 添加随机种子

        // 拿到boardcontainer的布局然后添加
        QLayout *layout = ui->boardContainer->layout();
        if (!layout)
        {
                layout = new QVBoxLayout(ui->boardContainer);
                ui->boardContainer->setLayout(layout);
        }
        layout->addWidget(m_boardWidget);

        // 3. 连接信号
        connect(m_boardWidget, &BoardWidget::piecePlaced,
                this, &MainWindow::onPiecePlaced);
        connect(m_boardWidget, &BoardWidget::gameOver,
                this, &MainWindow::onGameOver);

        // 4. 连接按钮
        connect(ui->saveFirstMoveButton, &QPushButton::clicked,
                this, &MainWindow::onSaveFirstMove);
        connect(ui->newGameButton, &QPushButton::clicked, this, &MainWindow::onRestart);
        // 5. 初始化状态栏和信息区域
        statusBar()
            ->showMessage("当前玩家: 黑棋");
        updateInfoText("游戏开始，黑棋先走");
}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::updateInfoText(const QString &text)
{
        ui->infoTextEdit->appendPlainText(text);
}

void MainWindow::onSaveFirstMove()
{
        int index = ui->firstMoveCombo->currentIndex(); // 0黑先，1白先，2随机
        int firstPlayer = 1;                            // 默认黑先

        if (index == 0)
        {
                firstPlayer = 1;
        }
        else if (index == 1)
        {
                firstPlayer = 2;
        }
        else if (index == 2)
        {
                // 随机生成 1 或 2
                firstPlayer = (std::rand() % 2) + 1;
        }

        m_game->setNextAiColor(firstPlayer); // 设置先手
        m_game->initBoard();                 // 清空棋盘
        m_boardWidget->update();             // 刷新显示

        QString playerStr = (firstPlayer == 1) ? "黑棋" : "白棋";
        statusBar()->showMessage("当前玩家: " + playerStr);
        updateInfoText("先手设置为 " + playerStr + "，游戏重新开始");
}

void MainWindow::onPiecePlaced(int nextPlayer)
{
        QString playerStr = (nextPlayer == 1) ? "黑棋" : "白棋";
        statusBar()->showMessage("当前玩家: " + playerStr);
        updateInfoText("轮到 " + playerStr);
}

void MainWindow::onGameOver(int winner)
{
        QString msg;
        if (winner == 1)
        {
                msg = "黑棋获胜！";
        }
        else if (winner == 2)
        {
                msg = "白棋获胜！";
        }
        else
        {
                msg = "平局！";
        }

        QMessageBox::information(this, "游戏结束", msg);
        updateInfoText(msg);

        // 禁用棋盘，防止继续下棋
        // m_boardWidget->setEnabled(false);
}

void MainWindow::onRestart()
{

        m_game->initBoard();     // 清空棋盘
        m_boardWidget->update(); // 刷新显示

        QString playerStr = (m_game->getCurrentPlayer() == 1) ? "黑棋" : "白棋";
        statusBar()->showMessage("当前玩家: " + playerStr);
        updateInfoText("游戏重新开始");
}
