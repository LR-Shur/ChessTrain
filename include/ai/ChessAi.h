#ifndef AI_H
#define AI_H
#include "game/ChessGame.h"
using namespace std;
#include <vector>
#include <algorithm>  // std::sort
#include <functional> // std::greater, std::less
#include <chrono>     //计时器

class ChessAi
{
private:
    ChessGame *chessGame;
    // 工具函数，根据棋型权重返回棋型分数
    int patternScore(int length, bool blockedStart, bool blockedEnd);

    int alphaBeta(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                  int depth,
                  int alpha,
                  int beta,
                  int player,
                  std::vector<int> &bestMove, bool isRoot);

    vector<vector<int>> generateMoves(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    int evaluateDirection(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    // 辅助：模拟落子（给临时棋盘落子）
    void placePieceInTempBoard(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                               int x, int y, int player);

    void copyBoard(const int (&src)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                   int (&dst)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);
    // 排序待走棋
    void SortMoves(vector<vector<int>> &moves, int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int playerColor, bool maximinzing);
    // 评估下这一步棋在周围局部能形成的内容
    int evaluateMove(vector<int> &move, int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int playerColor);

    const int FIVE = 100000;
    const int LIVE_FOUR = 10000;
    const int SLEEP_FOUR = 3000;
    const int LIVE_THREE = 1000;
    const int SLEEP_THREE = 200;
    const int LIVE_TWO = 200;
    const int SLEEP_TWO = 50;
    const int LIVE_ONE = 10;
    const int SLEEP_ONE = 1;

public:
    ChessAi(ChessGame *chessgame);
    ~ChessAi();

    std::vector<int> getBestMove(int aiColor = 1, int maxDepth = 6, int minDepth = 2, int timeLimitMs = 60000); // ms是毫秒
};

inline ChessAi::ChessAi(ChessGame *chessgame)
{
    chessGame = chessgame;
}

inline ChessAi::~ChessAi()
{
}

#endif