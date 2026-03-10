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
    bool calculated[SIZE][SIZE][4] = {false};
    int aiColor = chessGame->getAiColor();
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
                continue;

            int player = board[i][j];
            for (int d = 0; d < 4; d++)
            {
                if (calculated[i][j][d])
                    continue;

                int dx = dirs[d][0];
                int dy = dirs[d][1];

                int prevX = i - dx;
                int prevY = j - dy;
                // 检测是不是该方向的起点
                if (prevX >= 0 && prevX < SIZE && prevY >= 0 && prevY < SIZE && board[prevX][prevY] == player)
                {
                    continue;
                }
                int dirScore = calcWindowScore(i, j, dx, dy, board);

                // 标记已计算
                int markStep = 0;
                while (true)
                {
                    int nx = i + dx * markStep;
                    int ny = j + dy * markStep;
                    if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE || board[nx][ny] != player)
                        break;
                    calculated[nx][ny][d] = true;
                    markStep++;
                }

                // 黑白差异化加权
                if (player == 1)
                {
                    blackScore += dirScore; // 黑棋：进攻优先
                }
                else
                {
                    whiteScore += dirScore * 1.2; // 白棋：防守加权
                }
            }
        }
    }

    if (aiColor == 2)
    {
        return whiteScore - blackScore;
    }
    else
    {
        return blackScore - whiteScore;
    }
}

int ChessAi::alphaBeta(int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int depth, int alpha, int beta, int player, std::vector<int> &bestMove, bool isRoot, vector<vector<int>> lastbestMoves)
{
    constexpr int size = ChessGame::CHESSBOARDSIZE;
    int aiColor = chessGame->getAiColor();
    int opponentColor = (aiColor == 1) ? 2 : 1;
    int nextPlayer = (player == 1) ? 2 : 1;

    if (isRoot && depth >= 1)
    {
        vector<vector<int>> killMoves = findKillMoves(board, opponentColor);
        if (!killMoves.empty())
        {
            bestMove = killMoves[0]; // 直接堵杀棋点
            // std::cout << "[紧急防守] 检测到一步杀，防守点：(" << bestMove[0] << "," << bestMove[1] << ")" << std::endl;
            return aiColor == 1 ? -1000000 : 1000000;
        }
    }

    if (depth == 0 || checkWin(board) != 0)
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
        if (isRoot && !lastbestMoves.empty())
        {
            moves = lastbestMoves;
        }
        else
        {
            SortMoves(moves, board, player, true);
        }

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
            placePieceInTempBoard(tempBoard, move[0], move[1], player);
            int Score = alphaBeta(tempBoard, depth - 1, alpha, beta, nextPlayer, bestMove, false);

            if (Score > maxScore)
            {
                maxScore = Score;
                if (isRoot == true)
                {
                    bestMove = {move[0], move[1]};
                }
            }

            alpha = max(alpha, Score);
            if (beta <= alpha)
            {
                break;
            }
        }
        return maxScore;
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
            int Score = alphaBeta(tempBoard, depth - 1, alpha, beta, nextPlayer, bestMove, false);

            if (Score < minScore)
            {
                minScore = Score;
            }

            beta = min(beta, Score);
            if (beta <= alpha)
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
    int totalScore = 0; // 最终得分（进攻+防守）
    int opponentPlayer = playerColor == 1 ? 2 : 1;
    int x = move[0], y = move[1]; // 落子点坐标

    // 四个方向遍历
    for (int d = 0; d < 4; d++)
    {
        int dx = dirs[d][0];
        int dy = dirs[d][1];

        int selfForward = 0, selfForwardEmpty = 0;
        for (int i = 1; i < 5; i++)
        {
            int nx = x + dx * i;
            int ny = y + dy * i;
            if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
                break;
            if (board[nx][ny] == playerColor)
                selfForward++;
            else if (board[nx][ny] == 0)
                selfForwardEmpty++;
            else
                break; // 对方棋子，阻断
        }
        // 反方向（含空）统计
        int selfBackward = 0, selfBackwardEmpty = 0;
        for (int i = -1; i > -5; i--)
        {
            int nx = x + dx * i;
            int ny = y + dy * i;
            if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
                break;
            if (board[nx][ny] == playerColor)
                selfBackward++;
            else if (board[nx][ny] == 0)
                selfBackwardEmpty++;
            else
                break;
        }
        // 自己的总长度（含落子点）
        int selfTotalLen = selfForward + selfBackward + 1;
        // 自己的空位总数（判断是否是活棋）
        int selfTotalEmpty = selfForwardEmpty + selfBackwardEmpty;

        // 进攻分赋值（排序用，分数差拉开，优先搜高分）
        int attackScore = 0;
        if (selfTotalLen >= 5)
            attackScore = 100000; // 五连（必胜）
        else if (selfTotalLen == 4)
        {
            if (selfTotalEmpty >= 1)
                attackScore = 10000; // 活四
            else
                attackScore = 1000; // 冲四
        }
        else if (selfTotalLen == 3)
        {
            if (selfTotalEmpty >= 2)
                attackScore = 1000; // 活三（优先级高）
            else
                attackScore = 100; // 眠三
        }
        else if (selfTotalLen == 2)
        {
            if (selfTotalEmpty >= 2)
                attackScore = 100; // 活二
            else
                attackScore = 10; // 眠二
        }
        else
            attackScore = 1; // 单棋（保底分）

        board[x][y] = playerColor;

        // 统计对方在该方向的棋型
        int oppForward = 0, oppForwardEmpty = 0;
        for (int i = 1; i < 5; i++)
        {
            int nx = x + dx * i;
            int ny = y + dy * i;
            if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
                break;
            if (board[nx][ny] == opponentPlayer)
                oppForward++;
            else if (board[nx][ny] == 0)
                oppForwardEmpty++;
            else
                break;
        }
        int oppBackward = 0, oppBackwardEmpty = 0;
        for (int i = -1; i > -5; i--)
        {
            int nx = x + dx * i;
            int ny = y + dy * i;
            if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
                break;
            if (board[nx][ny] == opponentPlayer)
                oppBackward++;
            else if (board[nx][ny] == 0)
                oppBackwardEmpty++;
            else
                break;
        }
        // // 恢复落子点（避免修改原棋盘）
        // board[x][y] = 0;

        // 对方的总长度
        int oppTotalLen = oppForward + oppBackward + 1;
        int oppTotalEmpty = oppForwardEmpty + oppBackwardEmpty;

        // 防守分赋值（关键：堵对方活四/冲四的分远高于自己造棋型）
        int defendScore = 0;
        if (oppTotalLen >= 5)
            defendScore = 100000; // 堵五连（必防）
        else if (oppTotalLen == 4)
        {
            if (oppTotalEmpty >= 1)
                defendScore = 20000; // 堵活四（最高优先级）
            else
                defendScore = 2000; // 堵冲四（次高优先级）
        }
        else if (oppTotalLen == 3)
        {
            if (oppTotalEmpty >= 2)
                defendScore = 2000; // 堵活三（优先级高于自己活三）
            else
                defendScore = 200; // 堵眠三
        }
        else if (oppTotalLen == 2)
        {
            if (oppTotalEmpty >= 2)
                defendScore = 200; // 堵活二
            else
                defendScore = 20; // 堵眠二
        }
        else
            defendScore = 0; // 无防守价值

        // ======================
        // 3. 总得分 = 进攻分 + 防守分（防守分权重更高）
        // ======================
        totalScore += attackScore + defendScore;
    }

    board[x][y] = 0;
    return totalScore;
}

int ChessAi::checkWin(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE])
{
    for (int i = 0; i < ChessGame::CHESSBOARDSIZE; i++)
    {
        for (int j = 0; j < ChessGame::CHESSBOARDSIZE; j++)
        {
            if (board[i][j] == 0)
            {
                continue;
            }
            else
            {

                int result = checkWinPiece(i, j, board);
                if (result != 0)
                {
                    return result;
                }
            }
        }
    }
    return 0;
}

int ChessAi::checkWinPiece(int x, int y, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE])
{
    if (board[x][y] == 0)
    {
        return -1;
    }
    int color = board[x][y];
    for (const auto &dir : ChessGame::CheckDIRECTIONS)
    {
        if (checkDirection(x, y, dir[0], dir[1], color, board) >= 5)
        {
            return color;
        }
    }
    return 0;
}
int ChessAi::checkDirection(int x, int y, int dx, int dy, int color, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE])
{
    int count = 1; // 先算上自己这个点
    int size = ChessGame::CHESSBOARDSIZE;
    // 正方向
    int nx = x + dx;
    int ny = y + dy;
    while (nx >= 0 && nx < size &&
           ny >= 0 && ny < size &&
           board[nx][ny] == color)
    {
        ++count;
        nx += dx;
        ny += dy;
    }

    // 反方向
    nx = x - dx;
    ny = y - dy;
    while (nx >= 0 && nx < size &&
           ny >= 0 && ny < size &&
           board[nx][ny] == color)
    {
        ++count;
        nx -= dx;
        ny -= dy;
    }

    return count;
}

void ChessAi::SortMoves(vector<vector<int>> &moves, int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int playerColor, bool maximizing)
{
    vector<pair<int, vector<int>>> scoredMoves;
    for (auto &move : moves)
    {
        int score = evaluateMove(move, board, playerColor);
        scoredMoves.emplace_back(score, move);
    }
    std::sort(scoredMoves.begin(), scoredMoves.end(), greater<pair<int, vector<int>>>());
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

std::vector<int> ChessAi::getBestMove(int aiColor, int maxDepth, int minDepth, int timeLimitMs)
{
    const int SIZE = ChessGame::CHESSBOARDSIZE;
    std::vector<int> bestMove = {-1, -1}; // 初始无效位置

    std::vector<int> lastBestMove;

    int currentBoard[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            currentBoard[i][j] = chessGame->getPieceAt(i, j);
        }
    }
    // 计时器
    auto startTime = std::chrono::steady_clock::now();

    for (int depth = minDepth; depth <= maxDepth; depth++)
    {
        int alpha = -1000000; // 阿尔法初始值（极小）
        int beta = 1000000;   // 贝塔初始值（极大）
        // 检查是否超时
        auto now = std::chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
        if (elapsed >= timeLimitMs)
        {
            std::cout << "时间用完，停止于深度 " << depth - 1 << std::endl;
            break;
        }

        std::cout << "正在搜索深度 " << depth << " ..." << std::endl;

        // 生成走法（可能需要排序）
        auto moves = generateMoves(currentBoard);
        if (moves.empty())
            break;

        // 如果有上一深度的最佳走法且在当前 moves 中，将其移到最前面
        if (!lastBestMove.empty())
        {
            auto it = std::find(moves.begin(), moves.end(), lastBestMove);
            if (it != moves.end())
            {
                std::rotate(moves.begin(), it, it + 1); // 将 lastBestMove 换到开头
            }
        }

        // 对走法进行排序,排除bestmove
        auto firstMove = moves.front();
        moves.erase(moves.begin());
        SortMoves(moves, currentBoard, aiColor, true); // true 表示极大层
        moves.insert(moves.begin(), firstMove);

        // 本次搜索的最佳走法（临时）
        std::vector<int> currentBestMove;

        int score = alphaBeta(currentBoard, depth, alpha, beta, aiColor, currentBestMove, true, moves);

        if (!currentBestMove.empty())
        {
            bestMove = currentBestMove;
            lastBestMove = currentBestMove; // 记录作为下一深度的优先走法
        }

        std::cout << "深度 " << depth << " 最佳走法: (" << bestMove[0] << "," << bestMove[1] << ") 分数: " << score << std::endl;
    }

    return bestMove;
}

// 输入：方向起点、方向、棋盘、当前玩家
// 输出：该方向上所有5格窗口的总分（进攻+防守）
int ChessAi::calcWindowScore(int x, int y, int dx, int dy, const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE])
{
    const int SIZE = ChessGame::CHESSBOARDSIZE;
    int totalScore = 0;
    int windowLen = 5; // 五子棋核心：5格窗口

    // 遍历当前方向上的所有可能的5格窗口
    for (int step = 0; step <= 10; step++)
    {                        // 15格棋盘，每个方向最多10个窗口
        string pattern = ""; // 窗口内的棋型编码
        bool outOfBound = false;

        // 构建当前窗口的编码（5格）
        for (int w = 0; w < windowLen; w++)
        {
            int nx = x + dx * (step + w);
            int ny = y + dy * (step + w);
            if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
            {
                outOfBound = true;
                break;
            }
            // 转成字符：0=空，1=黑，2=白
            pattern += to_string(board[nx][ny]);
        }

        if (outOfBound)
            break; // 窗口出界，停止

        // 匹配棋型分数（进攻分+防守分）
        if (PATTERN_SCORE.count(pattern))
        {
            totalScore += PATTERN_SCORE.at(pattern);
        }
    }
    return totalScore;
}

vector<vector<int>> ChessAi::findKillMoves(const int (&board)[ChessGame::CHESSBOARDSIZE][ChessGame::CHESSBOARDSIZE], int opponentColor)
{
    vector<vector<int>> killMoves;
    const int SIZE = ChessGame::CHESSBOARDSIZE;
    vector<vector<int>> allEmpty = generateMoves(board);

    for (auto &move : allEmpty)
    {
        int x = move[0], y = move[1];
        // 1. 模拟对手下这步，检测是否直接五连（一步杀）
        int tempBoard[SIZE][SIZE];
        copyBoard(board, tempBoard);
        tempBoard[x][y] = opponentColor;
        if (checkWin(tempBoard) == opponentColor)
        {
            killMoves.push_back(move);
            continue;
        }

        // 2. 检测是否形成活四（下这步后，对手再下一次就五连）
        bool isLiveFour = false;
        const int dirs[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        for (int d = 0; d < 4; d++)
        {
            int dx = dirs[d][0], dy = dirs[d][1];
            // 统计当前方向的连续同色+空位
            int forward = 0, forwardEmpty = 0;
            int backward = 0, backwardEmpty = 0;

            // 正方向
            for (int i = 1; i < 5; i++)
            {
                int nx = x + dx * i, ny = y + dy * i;
                if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
                    break;
                if (tempBoard[nx][ny] == opponentColor)
                    forward++;
                else if (tempBoard[nx][ny] == 0)
                    forwardEmpty++;
                else
                    break;
            }
            // 反方向
            for (int i = 1; i < 5; i++)
            {
                int nx = x - dx * i, ny = y - dy * i;
                if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
                    break;
                if (tempBoard[nx][ny] == opponentColor)
                    backward++;
                else if (tempBoard[nx][ny] == 0)
                    backwardEmpty++;
                else
                    break;
            }

            // 活四判定：连续4个同色 + 至少1个空位
            int totalLen = forward + backward + 1;
            if (totalLen == 4 && (forwardEmpty + backwardEmpty) >= 1)
            {
                isLiveFour = true;
                break;
            }
        }

        if (isLiveFour)
        {
            killMoves.push_back(move);
        }
    }

    return killMoves;
}