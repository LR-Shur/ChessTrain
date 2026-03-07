#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include "game/ChessGame.h"

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWidget(ChessGame *game, QWidget *parent = nullptr);
    void setGame(ChessGame *game);

signals:
    void gameOver(int winner);
    void piecePlaced(int nextPlayer);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    ChessGame *m_game;
    // int m_cellSize; // 格子像素大小
    // int m_margin;   // 棋盘边距
    void calculateBoardParams(int &cellSize, int &margin) const;
};

#endif // BOARDWIDGET_H