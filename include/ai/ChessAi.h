#ifndef AI_H
#define AI_H
#include "game/ChessGame.h"
using namespace std;
#include <vector>
#include <algorithm>  // std::sort
#include <functional> // std::greater, std::less
#include <chrono>     //计时器
#include <unordered_map>

class ChessAi
{
private:
    ChessGame *chessGame;
    // 工具函数，根据棋型权重返回棋型分数
    // int patternScore(int length, bool blockedStart, bool blockedEnd);
    vector<vector<int>> findKillMoves(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int opponentColor);
    int alphaBeta(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                  int depth,
                  int alpha,
                  int beta,
                  int player,
                  std::vector<int> &bestMove, bool isRoot, vector<vector<int>> lastbestMoves = vector<vector<int>>());

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

    int checkWin(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    int checkWinPiece(int x, int y, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);
    int checkDirection(int x, int y, int dx, int dy, int color, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    int calcWindowScore(int x, int y, int dx, int dy, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    const unordered_map<string, int> PATTERN_SCORE = {
        // 必胜棋型（五连）
        {"11111", 1000000},
        {"22222", 1000000},
        // 活四（必防，包括单端被挡的情况）
        {"11110", 200000},
        {"01111", 200000}, // 无阻挡活四
        {"21111", 100000},
        {"11112", 100000}, // 单端被挡的活四（5格窗口截取）
        {"211110", 100000},
        {"011112", 100000}, // 6格完整活四（补充）
        // 冲四（次必防）
        {"11101", 80000},
        {"10111", 80000},
        {"22202", 70000},
        {"20222", 70000},
        // 绝对活三（下一端成活四）
        {"01110", 50000},
        {"02220", 50000},
        // 普通活三
        {"11100", 10000},
        {"00111", 10000},
        {"22200", 10000},
        {"00222", 10000},
        // 断三
        {"11010", 1000},
        {"10110", 1000},
        {"22020", 1000},
        {"20220", 1000},
        // 活二
        {"01100", 500},
        {"00110", 500},
        {"02200", 500},
        {"00220", 500},
        // 断二
        {"10100", 100},
        {"10010", 100},
        {"20200", 100},
        {"20020", 100},
        // 补全单端被挡的活三/活四变体
        {"011102", 80000},
        {"201110", 80000},
        {"022201", 90000},
        {"102220", 90000},
    };

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