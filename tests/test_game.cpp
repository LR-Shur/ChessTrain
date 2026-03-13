#include "game/chessgame.h"
#include "ai/ChessAi.h"
#include "../src/AZai/ai.h"

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include <fstream> // 引入文件操作头文件
#include <string>  // 字符串处理
#include <iomanip> // 格式化输出

using namespace std;

// 全局文件输出流（也可以作为函数参数传递）
ofstream resultFile;

static vector<int> getAzMove(ChessGame &game, int depth)
{
    int before[BOARD_SIZE][BOARD_SIZE] = {0};
    int pieceCount = 0;
    int current = game.getCurrentPlayer();

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            int piece = game.getPieceAt(row, col);
            Board[row][col] = piece;
            before[row][col] = piece;
            if (piece != 0)
            {
                ++pieceCount;
            }
        }
    }

    cot = pieceCount;
    ::aiColor = current;
    ::playerColor = (current == B) ? W : B;
    ::isGameOver = false;

    aiMove(depth, Board, current);

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            if (before[row][col] == EMPTY && Board[row][col] == current)
            {
                return {row, col};
            }
        }
    }

    return {-1, -1};
}

static int runOneGame(bool azBlack, int maxPlies = 80)
{
    ChessGame game;
    ChessAi originAi(&game);

    for (int ply = 0; ply < maxPlies; ++ply)
    {
        if (ply > 0 && ply % 10 == 0)
        {
            cout << "  进度: 已下 " << ply << " 手..." << endl;
        }

        int current = game.getCurrentPlayer();
        bool azTurn = (current == B) ? azBlack : !azBlack;

        vector<int> move;
        if (azTurn)
        {
            move = getAzMove(game, 3);
        }
        else
        {
            game.setNextAiColor(current);
            std::ostringstream sink;
            std::streambuf *oldBuf = std::cout.rdbuf(sink.rdbuf());
            move = originAi.getBestMove(current, 3, 2, 200000);
            std::cout.rdbuf(oldBuf);
        }

        if (move.size() < 2)
        {
            return 0;
        }
        if (!game.placePiece(move[0], move[1]))
        {
            return 0;
        }

        int winner = game.checkWinGlobal();
        if (winner != 0)
        {
            return winner;
        }
        if (game.isBoardFull())
        {
            return 0;
        }
    }
    return 0;
}

int main()
{
    constexpr int GAMES_PER_SIDE = 3;
    constexpr int TOTAL_GAMES = GAMES_PER_SIDE * 2;

    // 1. 打开文件（追加模式 | 输出模式，若文件不存在则创建）
    // 文件名可以自定义，比如 "chess_ai_results.txt"
    resultFile.open("chess_ai_results.txt", ios::out | ios::app);
    if (!resultFile.is_open())
    {
        // 如果文件打开失败，降级输出到控制台提示错误
        cerr << "错误：无法打开结果文件！" << endl;
        return 1;
    }

    int azWins = 0;
    int originWins = 0;
    int draws = 0;

    int azBlackWins = 0;
    int azBlackLosses = 0;
    int azBlackDraws = 0;

    int originBlackWins = 0;
    int originBlackLosses = 0;
    int originBlackDraws = 0;

    // 写入文件头（标记本次运行时间/批次）
    resultFile << "==================== 新批次对局结果 ====================" << endl;

    // 前50局 AZai 执黑
    for (int i = 0; i < GAMES_PER_SIDE; ++i)
    {
        cout << "[AZ黑] 开始第 " << (i + 1) << " 局..." << endl;
        int winner = runOneGame(true);
        if (winner == B)
        {
            ++azWins;
            ++azBlackWins;
        }
        else if (winner == W)
        {
            ++originWins;
            ++azBlackLosses;
        }
        else
        {
            ++draws;
            ++azBlackDraws;
        }
        // 替换cout为resultFile（文件输出）
        resultFile << "[AZ黑] Game " << (i + 1) << " winner=" << winner << endl;
        cout << "[AZ黑] 第 " << (i + 1) << " 局结束, winner=" << winner << endl;
    }

    // 后50局 原始AI 执黑（即 AZai 执白）
    for (int i = 0; i < GAMES_PER_SIDE; ++i)
    {
        cout << "[原始AI黑] 开始第 " << (i + 1) << " 局..." << endl;
        int winner = runOneGame(false);
        if (winner == B)
        {
            ++originWins;
            ++originBlackWins;
        }
        else if (winner == W)
        {
            ++azWins;
            ++originBlackLosses;
        }
        else
        {
            ++draws;
            ++originBlackDraws;
        }
        // 替换cout为resultFile（文件输出）
        resultFile << "[原始AI黑] Game " << (i + 1) << " winner=" << winner << endl;
        cout << "[原始AI黑] 第 " << (i + 1) << " 局结束, winner=" << winner << endl;
    }

    // 汇总信息写入文件（保留格式化输出，比如胜率保留2位小数）
    resultFile << "=== 对局汇总(" << TOTAL_GAMES << "局) ===" << endl;
    resultFile << "AZai 胜: " << azWins << ", 原始AI 胜: " << originWins << ", 平局: " << draws << endl;
    resultFile << "AZai 总胜率: " << fixed << setprecision(2) << (azWins * 100.0 / TOTAL_GAMES) << "%" << endl;
    resultFile << "原始AI 总胜率: " << fixed << setprecision(2) << (originWins * 100.0 / TOTAL_GAMES) << "%" << endl;

    resultFile << "=== AZai 执黑(50局) ===" << endl;
    resultFile << "胜: " << azBlackWins << ", 负: " << azBlackLosses << ", 平: " << azBlackDraws << endl;
    resultFile << "胜率: " << fixed << setprecision(2) << (azBlackWins * 100.0 / GAMES_PER_SIDE) << "%" << endl;

    resultFile << "=== 原始AI 执黑(50局) ===" << endl;
    resultFile << "原始AI 胜: " << originBlackWins << ", 负: " << originBlackLosses << ", 平: " << originBlackDraws << endl;
    resultFile << "原始AI 执黑胜率: " << fixed << setprecision(2) << (originBlackWins * 100.0 / GAMES_PER_SIDE) << "%" << endl;

    cout << "=== 对局汇总(" << TOTAL_GAMES << "局) ===" << endl;
    cout << "AZai 胜: " << azWins << ", 原始AI 胜: " << originWins << ", 平局: " << draws << endl;
    cout << "AZai 总胜率: " << fixed << setprecision(2) << (azWins * 100.0 / TOTAL_GAMES) << "%" << endl;
    cout << "原始AI 总胜率: " << fixed << setprecision(2) << (originWins * 100.0 / TOTAL_GAMES) << "%" << endl;

    // 写入分隔符，方便区分不同批次
    resultFile << "========================================================" << endl
               << endl;

    // 2. 关闭文件（必须！否则内容可能无法写入）
    resultFile.close();

    // 可选：控制台提示文件写入完成
    cout << "对局结果已写入 chess_ai_results.txt 文件！" << endl;

    return 0;
}