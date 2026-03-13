#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BoardWidget.h"
#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent, ChessGame *game, ChessAi *ai)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_game(game), m_ai(ai), m_boardWidget(nullptr)
{
        ui->setupUi(this);
        // 创建棋盘
        m_boardWidget = new BoardWidget(m_game, this, ai);
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
                this, &MainWindow::onSaveAiMoveColor);
        connect(ui->newGameButton, &QPushButton::clicked, this, &MainWindow::onRestart);
        connect(ui->useAzAiCheckBox, &QCheckBox::toggled,
                this, &MainWindow::onUseAzAiToggled);
        connect(ui->aiVsAiCheckBox, &QCheckBox::toggled,
                this, &MainWindow::onAiVsAiToggled);

        // 默认不使用 AZai
        ui->useAzAiCheckBox->setChecked(false);
        ui->aiVsAiCheckBox->setChecked(false);
        ui->aiVsAiBlackCombo->setCurrentIndex(0);
        m_boardWidget->setUseAzAi(false);
        m_boardWidget->setAiVsAi(false);
        m_boardWidget->setAiVsAiAzBlack(false);
        // 5. 初始化状态栏和信息区域
        statusBar()
            ->showMessage("当前玩家: 黑棋");
        updateInfoText("游戏开始，黑棋先走");

        m_boardWidget->restart();
}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::updateInfoText(const QString &text)
{
        ui->infoTextEdit->appendPlainText(text);
}

void MainWindow::onSaveAiMoveColor()
{
        int index = ui->firstMoveCombo->currentIndex(); // 0黑先，1白先，2随机
        int aiPlayer = 1;                               // 默认黑先

        if (index == 0)
        {
                aiPlayer = 1;
        }
        else if (index == 1)
        {
                aiPlayer = 2;
        }
        else if (index == 2)
        {
                // 随机生成 1 或 2
                aiPlayer = (std::rand() % 2) + 1;
        }

        m_game->setNextAiColor(aiPlayer); // 人机时设置AI颜色

        // AI对下模式：设置黑方(先手)由哪种AI执子
        m_aiVsAiAzBlack = resolveAiVsAiAzBlack();
        m_boardWidget->setAiVsAiAzBlack(m_aiVsAiAzBlack);

        m_boardWidget->restart();

        QString aiStr = (aiPlayer == 1) ? "黑棋" : "白棋";
        QString blackAiStr = m_aiVsAiAzBlack ? "AZai执黑" : "原始AI执黑";
        statusBar()->showMessage("当前ai: " + aiStr);
        updateInfoText("ai设置为 " + aiStr + ", AI对下先手设置: " + blackAiStr + ", 游戏重新开始");
}

void MainWindow::onPiecePlaced(int nextPlayer)
{
        QString playerStr = (nextPlayer == 1) ? "黑棋" : "白棋";
        statusBar()->showMessage("当前玩家: " + playerStr);
        updateInfoText("轮到 " + playerStr);
}

void MainWindow::onUseAzAiToggled(bool checked)
{
        m_boardWidget->setUseAzAi(checked);
        if (ui->aiVsAiCheckBox->isChecked())
        {
                updateInfoText("提示: 已开启双方AI对下, 人机模式的 AZai 开关当前不生效");
                return;
        }
        QString mode = checked ? "AZai" : "原始AI";
        updateInfoText("AI模式切换为: " + mode);
}

void MainWindow::onAiVsAiToggled(bool checked)
{
        m_boardWidget->setAiVsAi(checked);
        m_aiVsAiAzBlack = resolveAiVsAiAzBlack();
        m_boardWidget->setAiVsAiAzBlack(m_aiVsAiAzBlack);
        QString mode = checked ? "双方AI对下(优先级高于AZai勾选)" : "人机对战";
        updateInfoText("对战模式切换为: " + mode);
}

bool MainWindow::resolveAiVsAiAzBlack() const
{
        int index = ui->aiVsAiBlackCombo->currentIndex();
        if (index == 1)
        {
                return true;
        }
        if (index == 2)
        {
                return (std::rand() % 2) == 1;
        }
        return false;
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
        m_boardWidget->restart();

        QString playerStr = (m_game->getCurrentPlayer() == 1) ? "黑棋" : "白棋";
        statusBar()->showMessage("当前玩家: " + playerStr);
        updateInfoText("游戏重新开始");
}
