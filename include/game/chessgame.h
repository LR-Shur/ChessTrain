#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <iostream>
#include <vector>
using namespace std;

class ChessGame
{
private:
    static const int CHESSBOARDSIZE = 15;
    static const int BLACKPLAYER = 1;
    static const int WHITEPLAYER = 2;
    inline static const int CheckDIRECTIONS[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}}; // 用于检查胜利的四个方向
    int board[CHESSBOARDSIZE][CHESSBOARDSIZE];                                         // 左边是x，右边是y
    int currentPlayer;                                                                 // 当前颜色
    bool gameOver;
    int winner;

    void changePlayer();
    // 单方向检查：从 (x,y) 出发，沿 (dx,dy) 两边扩展，返回连着的数量
    int checkDirection(int x, int y, int dx, int dy, int color);

public:
    ChessGame();
    void initBoard();
    bool placePiece(int x, int y);
    // 沿着四个方向检查是否胜利
    int checkWin(int x, int y);
    // 检测整个棋盘，是否有胜利
    int checkWinGlobal();
    bool isBoardFull();
    int getCurrentPlayer();
};

#endif