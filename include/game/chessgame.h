#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <iostream>
#include <vector>
using namespace std;

class ChessGame
{
private:
    static const int BLACKPLAYER = 1;
    static const int WHITEPLAYER = 2;
    
    int board[15][15];

    int currentPlayer;                                                                 // 当前颜色
    bool gameOver;
    int winner;
    int aiColor = 1;

    void changePlayer();
    // 单方向检查：从 (x,y) 出发，沿 (dx,dy) 两边扩展，返回连着的数量
    int checkDirection(int x, int y, int dx, int dy, int color);

public:
    static const int CHESSBOARDSIZE = 15;
    inline static const int CheckDIRECTIONS[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}}; // 用于检查胜利的四个方向
    ChessGame();

    void initBoard();

    bool placePiece(int x, int y);

    // 沿着四个方向检查是否胜利
    int checkWin(int x, int y);

    // 检测整个棋盘，是否有胜利
    int checkWinGlobal();

    // 检查是否满了
    bool isBoardFull();

    // 获取当前玩家颜色
    int getCurrentPlayer();

    // 获取指定位置的棋子颜色（0 空，1 黑，2 白）
    int getPieceAt(int x, int y) const;

    // 获取棋盘大小
    static const int getBoardSize() { return CHESSBOARDSIZE; }

    // 设置ai下一局颜色
    void setNextAiColor(int color);

    int getAiColor();

    void undoPiece(int x, int y);
};

#endif