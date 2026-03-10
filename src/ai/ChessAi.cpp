#include "ai/ChessAi.h"

inline vector<vector<int>> ChessAi::generateMoves(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE])
{
    const int SIZE = ChessGame::CHESSBOARDSIZE;
    vector<vector<int>> result;
    bool visited[SIZE][SIZE] = {false};

    // 八个方向偏移
    const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {

            if (board[i][j] != 0)
            {
                for (int z = 0; z < 8; z++)
                {
                    int ni = i + dx[z];
                    int nj = j + dy[z];
                    if (ni >= 0 && ni < SIZE && nj >= 0 && nj < SIZE)
                    {
                        if (board[ni][nj] == 0 && !visited[ni][nj])
                        {
                            visited[ni][nj] = true;
                            result.push_back({ni, nj});
                        }
                    }
                }
            }
        }
    }
    // 棋盘为空就下中间
    if (result.empty())
    {
        result.push_back({SIZE / 2, SIZE / 2});
    }
    // cout << result.size();
    return result;
}

int ChessAi::evaluateDirection(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE])
{
    int blackScore = 0;
    int whiteScore = 0;
    const int SIZE = ChessGame::CHESSBOARDSIZE;
    const int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
                continue;

            int player = board[i][j];
            for (int d = 0; d < 4; d++)
            {
                int dx = dirs[d][0];
                int dy = dirs[d][1];

                int prevX = i - dx;
                int prevY = j - dy;
                // 检测是不是该方向的起点
                if (prevX >= 0 && prevX < SIZE && prevY >= 0 && prevY < SIZE && board[prevX][prevY] == player)
                {
                    continue;
                }

                // 统计正方向连续同色长度
                int len = 1;
                int step = 1;
                while (true)
                {
                    int nx = i + dx * step;
                    int ny = j + dy * step;
                    if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
                        break;
                    if (board[nx][ny] == player)
                    {
                        len++;
                        step++;
                    }
                    else
                    {
                        break;
                    }
                }

                // 检查正方向终点是否被阻挡（出界或有其他棋子）
                int nxEnd = i + dx * step;
                int nyEnd = j + dy * step;
                bool blockedEnd = (nxEnd < 0 || nxEnd >= SIZE || nyEnd < 0 || nyEnd >= SIZE) || (board[nxEnd][nyEnd] != 0);

                // 检查反方向起点前一个位置是否被阻挡
                int nxStart = i - dx;
                int nyStart = j - dy;
                bool blockedStart = (nxStart < 0 || nxStart >= SIZE || nyStart < 0 || nyStart >= SIZE) || (board[nxStart][nyStart] != 0);

                // 计算此段棋型的分数
                int score = patternScore(len, blockedStart, blockedEnd);

                // 累加到对应颜色总分
                if (player == 1)
                {
                    blackScore += score;
                }
                else
                {
                    whiteScore += score;
                }
            }
        }
    }

    return blackScore - whiteScore; // 黑方得分减去白方得分，正数表示黑优
}

inline int ChessAi::patternScore(int length, bool blockedStart, bool blockedEnd)
{
    if (length >= 5)
        return FIVE;

    if (length == 4)
    {
        if (!blockedStart && !blockedEnd)
            return LIVE_FOUR;
        else if (!blockedStart || !blockedEnd)
            return SLEEP_FOUR; // 单边阻挡
        else
            return 0; // 两边阻挡
    }
    else if (length == 3)
    {
        if (!blockedStart && !blockedEnd)
            return LIVE_THREE;
        else if (!blockedStart || !blockedEnd)
            return SLEEP_THREE;
        else
            return 0;
    }
    else if (length == 2)
    {
        if (!blockedStart && !blockedEnd)
            return LIVE_TWO;
        else if (!blockedStart || !blockedEnd)
            return SLEEP_TWO;
        else
            return 0;
    }
    else if (length == 1)
    {
        if (!blockedStart && !blockedEnd)
            return LIVE_ONE;
        else if (!blockedStart || !blockedEnd)
            return SLEEP_ONE;
        else
            return 0;
    }
    return 0;
}
int ChessAi::alphaBeta(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int depth, int alpha, int beta, int player, std::vector<int> &bestMove, bool isRoot)
{
    constexpr int size = ChessGame::CHESSBOARDSIZE;
    int aiColor = chessGame->getAiColor();
    int opponentColor = (aiColor == 1) ? 2 : 1;
    if (depth == 0 || chessGame->checkWinGlobal() != 0)
    {
        return evaluateDirection(board);
    }

    auto moves = generateMoves(board);
    if (moves.empty())
    {
        return evaluateDirection(board); // 无棋可下
    }

    int remaining = 0;
    if (isRoot)
    {
        remaining = moves.size();
        std::cout << "[AI 搜索] 总候选点: " << remaining << std::endl;
    }
    // 极大
    if (aiColor == player)
    {
        // 排序move
        SortMoves(moves, board, player, true);

        long long int maxScore = -10000000000;
        for (auto &move : moves)
        {

            if (isRoot)
            {
                remaining--;
                std::cout << "搜索 (" << move[0] << "," << move[1]
                          << ")  剩余: " << remaining << std::endl;
            }
            int tempBoard[size][size];
            copyBoard(board, tempBoard);
            placePieceInTempBoard(tempBoard, move[0], move[1], aiColor);
            int Score = alphaBeta(tempBoard, depth - 1, alpha, beta, opponentColor, bestMove, false);

            if (Score > maxScore)
            {
                maxScore = Score;
                if (isRoot == true)
                {
                    bestMove = {move[0], move[1]};
                }
            }

            alpha = max(alpha, Score);
            if (beta < alpha)
            {
                break;
            }
        }
        return alpha;
    }
    else // 极小
    {

        SortMoves(moves, board, player, false);

        long long int minScore = 10000000000;

        for (auto &move : moves)
        {
            int tempBoard[size][size];
            copyBoard(board, tempBoard);
            placePieceInTempBoard(tempBoard, move[0], move[1], player);
            int Score = alphaBeta(tempBoard, depth - 1, alpha, beta, opponentColor, bestMove, false);

            if (Score < minScore)
            {
                minScore = Score;
            }

            beta = min(beta, Score);
            if (beta < alpha)
            {
                break;
            }
        }
        return minScore;
    }
}

// 复制棋盘（避免修改原游戏的棋盘）
void ChessAi::copyBoard(const int (&src)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                        int (&dst)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE])
{
    const int SIZE = ChessGame::CHESSBOARDSIZE;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            dst[i][j] = src[i][j];
        }
    }
}

int ChessAi::evaluateMove(vector<int> &move, int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int playerColor)
{
    const int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    int SIZE = ChessGame::CHESSBOARDSIZE;
    int score = 0;
    int opponentPlayer = playerColor == 1 ? 2 : 1;
    // 四个方向
    for (int d = 0; d < 4; d++)
    {
        int dx = dirs[d][0];
        int dy = dirs[d][1];

        // 统计方向连续同色长度

        bool isBlockStart = false; // 起点是否被阻碍
        bool isBlockEnd = false;   // 终点是否被阻碍

        // 先正方向
        int lenForward = 0; // 不含本体
        int lenBackward = 0;
        for (int i = 1; i < 5; i++)
        {
            int nx = move[0] + dx * i;
            int ny = move[1] + dy * i;

            if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
            {
                isBlockEnd = true;
                break;
            }
            if (board[nx][ny] == playerColor)
            {
                lenForward++;
            }
            else if (board[nx][ny] == opponentPlayer)
            {
                isBlockEnd = true;
                break;
            }
            else
            {
                break;
            }
        }

        // 反方向
        for (int i = -1; i > -5; i--)
        {
            int nx = move[0] + dx * i;
            int ny = move[1] + dy * i;

            if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
            {
                isBlockStart = true;
                break;
            }
            if (board[nx][ny] == playerColor)
            {
                lenBackward++;
            }
            else if (board[nx][ny] == opponentPlayer)
            {
                isBlockStart = true;
                break;
            }
            else
            {
                break;
            }
        }
        int len = lenForward + lenBackward + 1;
        score += patternScore(len, isBlockStart, isBlockEnd);
    }

    return score;
}

void ChessAi::SortMoves(vector<vector<int>> &moves, int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int playerColor, bool maximizing)
{
    vector<pair<int, vector<int>>> scoredMoves;
    for (auto &move : moves)
    {
        int score = evaluateMove(move, board, playerColor);
        scoredMoves.emplace_back(score, move);
    }
    if (maximizing)
    {
        // 极大
        std::sort(scoredMoves.begin(), scoredMoves.end(), greater<pair<int, vector<int>>>());
    }
    else
    {
        std::sort(scoredMoves.begin(), scoredMoves.end(), less<pair<int, vector<int>>>());
    }
    // 更新 moves
    moves.clear();
    for (auto &p : scoredMoves)
    {
        moves.push_back(p.second);
    }
}

// 给临时棋盘落子（模拟AI/玩家落子）
void ChessAi::placePieceInTempBoard(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE],
                                    int x, int y, int player)
{
    if (x >= 0 && x < ChessGame::CHESSBOARDSIZE && y >= 0 && y < ChessGame::CHESSBOARDSIZE)
    {
        board[x][y] = player;
    }
}

std::vector<int> ChessAi::getBestMove(int aiColor, int depth)
{
    const int SIZE = ChessGame::CHESSBOARDSIZE;
    std::vector<int> bestMove = {-1, -1}; // 初始无效位置
    int alpha = -1000000;                 // 阿尔法初始值（极小）
    int beta = 1000000;                   // 贝塔初始值（极大）

    int currentBoard[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            currentBoard[i][j] = chessGame->getPieceAt(i, j);
        }
    }

    alphaBeta(currentBoard, depth, alpha, beta, aiColor, bestMove, true);
    if (bestMove[0] == -1 || bestMove[1] == -1)
    {
        bestMove = {SIZE / 2, SIZE / 2};
    }

    return bestMove;
}
