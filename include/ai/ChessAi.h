#ifndef AI_H
#define AI_H
#include "game/ChessGame.h"
using namespace std;
#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>
#include <unordered_map>

class ChessAi
{
private:
    ChessGame *chessGame;
    vector<vector<int>> findKillMoves(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int opponentColor);
    int alphaBeta(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                  int depth,
                  int alpha,
                  int beta,
                  int player,
                  std::vector<int> &bestMove, bool isRoot, vector<vector<int>> lastbestMoves = vector<vector<int>>());

    vector<vector<int>> generateMoves(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);
    int evaluateDirection(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    void placePieceInTempBoard(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                               int x, int y, int player);

    void copyBoard(const int (&src)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                   int (&dst)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    void SortMoves(vector<vector<int>> &moves, int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int playerColor, bool maximinzing);
    int evaluateMove(vector<int> &move, int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int playerColor);

    int checkWin(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);
    int checkWinPiece(int x, int y, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);
    int checkDirection(int x, int y, int dx, int dy, int color, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);
    int calcWindowScore(int x, int y, int dx, int dy, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE]);

    const unordered_map<string, int> PATTERN_SCORE = {
        {"11111", 1000000},
        {"22222", 1000000},
        {"11110", 200000},
        {"01111", 200000},
        {"21111", 100000},
        {"11112", 100000},
        {"211110", 100000},
        {"011112", 100000},
        {"11101", 80000},
        {"10111", 80000},
        {"22202", 70000},
        {"20222", 70000},
        {"01110", 50000},
        {"02220", 50000},
        {"11100", 10000},
        {"00111", 10000},
        {"22200", 10000},
        {"00222", 10000},
        {"11010", 1000},
        {"10110", 1000},
        {"22020", 1000},
        {"20220", 1000},
        {"01100", 500},
        {"00110", 500},
        {"02200", 500},
        {"00220", 500},
        {"10100", 100},
        {"10010", 100},
        {"20200", 100},
        {"20020", 100},
        {"011102", 80000},
        {"201110", 80000},
        {"022201", 90000},
        {"102220", 90000},
    };

public:
    ChessAi(ChessGame *chessgame);
    ~ChessAi();

    std::vector<int> getBestMove(int aiColor = 1, int maxDepth = 6, int minDepth = 2, int timeLimitMs = 60000);
};

inline ChessAi::ChessAi(ChessGame *chessgame)
{
    chessGame = chessgame;
}

inline ChessAi::~ChessAi()
{
}

#endif
