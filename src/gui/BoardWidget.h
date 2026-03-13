#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include "game/ChessGame.h"
#include "ai/ChessAi.h"
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWidget(ChessGame *game = nullptr, QWidget *parent = nullptr, ChessAi *chessAi = nullptr);
    void setGame(ChessGame *game);
    void setClickEnabled(bool enabled);
    void setUseAzAi(bool enabled);
    void setAiVsAi(bool enabled);
    void setAiVsAiAzBlack(bool azBlack);
    void restart();

signals:
    void gameOver(int winner);
    void piecePlaced(int nextPlayer);

private slots:
    void onAiMove();
    void onAiMoveFinished(std::vector<int> aiMove);

protected:
    void
    paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    ChessGame *m_game;
    ChessAi *m_ai;
    QFuture<void> m_aiFuture;
    bool m_useAzAi = false;
    bool m_aiVsAi = false;
    bool m_aiVsAiAzBlack = false;
    bool m_clickEnabled = true; // 控制玩家点击
    bool m_isAiTurn = false;    // 标记是否是AI回合（防止重复触发）

    // 计算当前控件大小下最佳的格子尺寸和边距
    void calculateBoardParams(int &cellSize, int &margin) const;
    void checkGameOver();
    bool isCurrentPlayerAi() const;
    std::vector<int> getAzBestMove(int depth = 3) const;
};

#endif // BOARDWIDGET_H