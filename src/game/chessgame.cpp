#include "game/ChessGame.h"
#include "game/ChessGame.h"

void ChessGame::changePlayer()
{
    if (currentPlayer == BLACKPLAYER)
    {
        currentPlayer = WHITEPLAYER;
    }
    else if (currentPlayer == WHITEPLAYER)
    {
        currentPlayer = BLACKPLAYER;
    }
}

// 单方向检查：从 (x,y) 出发，沿 (dx,dy) 两边扩展，返回连着的数量
int ChessGame::checkDirection(int x, int y, int dx, int dy, int color)
{
    int count = 1; // 先算上自己这个点

    // 正方向
    int nx = x + dx;
    int ny = y + dy;
    while (nx >= 0 && nx < CHESSBOARDSIZE &&
           ny >= 0 && ny < CHESSBOARDSIZE &&
           board[nx][ny] == color)
    {
        ++count;
        nx += dx;
        ny += dy;
    }

    // 反方向
    nx = x - dx;
    ny = y - dy;
    while (nx >= 0 && nx < CHESSBOARDSIZE &&
           ny >= 0 && ny < CHESSBOARDSIZE &&
           board[nx][ny] == color)
    {
        ++count;
        nx -= dx;
        ny -= dy;
    }

    return count;
}

ChessGame::ChessGame()
{
    currentPlayer = BLACKPLAYER;
    gameOver = false;
    winner = false;
    initBoard();
}

void ChessGame::initBoard()
{
    for (int i = 0; i < CHESSBOARDSIZE; i++)
    {
        for (int j = 0; j < CHESSBOARDSIZE; j++)
        {
            board[i][j] = 0;
        }
    }
    currentPlayer = BLACKPLAYER; // 重启后，黑棋先动
}

bool ChessGame::placePiece(int x, int y)
{

    if (board[x][y] != 0)
    {
        return false;
    }
    else
    {
        board[x][y] = currentPlayer;
        changePlayer();
        return true;
    }
}

// 沿着四个方向检查是否胜利
int ChessGame::checkWin(int x, int y)
{
    if (board[x][y] == 0)
    {
        return -1;
    }
    int color = board[x][y];
    for (const auto &dir : CheckDIRECTIONS)
    {
        if (checkDirection(x, y, dir[0], dir[1], color) >= 5)
        {
            return color;
        }
    }
    return 0;
}

// 检测整个棋盘，是否有胜利
int ChessGame::checkWinGlobal()
{
    for (int i = 0; i < CHESSBOARDSIZE; i++)
    {
        for (int j = 0; j < CHESSBOARDSIZE; j++)
        {
            if (board[i][j] == 0)
            {
                continue;
            }
            else
            {

                int result = checkWin(i, j);
                if (result != 0)
                {
                    return result;
                }
            }
        }
    }
    return 0;
}

bool ChessGame::isBoardFull()
{
    for (int i = 0; i < CHESSBOARDSIZE; i++)
    {
        for (int j = 0; j < CHESSBOARDSIZE; j++)
        {
            if (board[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

int ChessGame::getCurrentPlayer()
{
    return currentPlayer;
}

int ChessGame::getPieceAt(int x, int y) const
{
    if (x < 0 || x >= CHESSBOARDSIZE || y < 0 || y >= CHESSBOARDSIZE)
    {
        return -1;
    }

    return board[x][y];
}

void ChessGame::setNextAiColor(int color)
{
    if (color == 1 || color == 2)
    {
        aiColor = color;
    }
}

int ChessGame::getAiColor()
{
    return aiColor;
}

void ChessGame::undoPiece(int x, int y)
{
    if (x >= 0 && x < CHESSBOARDSIZE && y >= 0 && y < CHESSBOARDSIZE)
    {
        board[x][y] = 0; // 清空该位置
        changePlayer();  // 还原玩家（因为placePiece会切换玩家）
    }
}