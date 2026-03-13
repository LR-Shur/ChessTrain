#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game/chessgame.h"
#include "BoardWidget.h"
#include "ai/ChessAi.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, ChessGame *chessGame = nullptr, ChessAi *chessAi = nullptr);
    ~MainWindow();

private:
    void updateInfoText(const QString &text);
    bool resolveAiVsAiAzBlack() const;

    Ui::MainWindow *ui;
    ChessGame *m_game;          // 游戏逻辑对象
    ChessAi *m_ai;
    BoardWidget *m_boardWidget; // 棋盘控件
    bool m_aiVsAiAzBlack = false;

private slots:
    void onRestart();                   // 重新开始游戏
    void onSaveAiMoveColor();           // 保存先手设置
    void onUseAzAiToggled(bool checked);
    void onAiVsAiToggled(bool checked);
    void onPiecePlaced(int nextPlayer); // 处理落子后更新界面
    void onGameOver(int winner);        // 处理游戏结束
};

#endif // MAINWINDOW_H
