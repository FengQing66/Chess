#include "pch.h"
#include "Game.h"
#include "Piece.h"
#include "Board.h"
#include <utility>

Game::Game() : m_selectedPiece(nullptr), m_isRedTurn(true)
{
    m_board.ResetBoard();
}

bool Game::IsValidCoord(int x, int y) const
{
    return x >= 0 && x < 9 && y >= 0 && y < 10;
}

void Game::CheckGameOver()
{
    bool redAlive = false;
    bool blackAlive = false;

    auto& board = m_board.GetBoardData();
    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 9; ++x)
        {
            Piece* p = board[y][x];
            if (!p || !p->IsAlive()) continue;

            if (p->GetType() == PieceType::GENERAL)
            {
                if (p->GetCamp() == Camp::RED) redAlive = true;
                if (p->GetCamp() == Camp::BLACK) blackAlive = true;
            }
        }
    }

    if (!redAlive || !blackAlive)
    {
        m_gameOver = true;
        m_winner = redAlive ? Camp::RED : Camp::BLACK;
    }
}

void Game::OnClick(int x, int y)
{
    if (m_gameOver) return;
    if (!IsValidCoord(x, y)) return;

    auto& board = m_board.GetBoardData();
    Piece* clicked = board[y][x];

    if (m_selectedPiece == nullptr)
    {
        // 选中己方棋子
        if (clicked && clicked->IsAlive() && clicked->GetCamp() == (m_isRedTurn ? Camp::RED : Camp::BLACK))
        {
            m_selectedPiece = clicked;
        }
    }
    else
    {
        if (clicked == m_selectedPiece)
        {
            m_selectedPiece = nullptr;
            return;
        }

        if (clicked && clicked->GetCamp() == m_selectedPiece->GetCamp())
        {
            m_selectedPiece = clicked;
            return;
        }

        std::pair<int, int> from = m_selectedPiece->GetPosition();
        if (m_selectedPiece->IsValidMove(x, y, board))
        {
            if (clicked && clicked->GetCamp() != m_selectedPiece->GetCamp())
            {
                clicked->Capture();
            }

            m_board.UpdatePiecePosition(from.first, from.second, x, y);

            // 这里主动调用 ApplyMove 并标记为本地发起
            ApplyMove(from.first, from.second, x, y, true);
        }
    }
}

//另一个对战的接收棋子信息
void Game::ApplyMove(int fromX, int fromY, int toX, int toY , bool isLocalMove)
{
    if (m_gameOver) return;

    Piece* moving = m_board.GetPieceAt(fromX, fromY);
    Piece* target = m_board.GetPieceAt(toX, toY);

    if (!moving || !moving->IsAlive()) return;

    // 吃子逻辑
    if (target && target->GetCamp() != moving->GetCamp()) {
        target->Capture();
    }

    // 更新棋盘
    m_board.UpdatePiecePosition(fromX, fromY, toX, toY);

    // 如果是本地玩家走子，则发送给对方
    if (isLocalMove && m_pNet && m_pNet->IsConnected()) {
        m_pNet->SendMove(fromX, fromY, toX, toY);
    }

    m_selectedPiece = nullptr;
    m_isRedTurn = !m_isRedTurn;
    CheckGameOver();

}
void Game::SetRedTurn(bool isRedTurn)
{
    m_isRedTurn = isRedTurn;
}


const Piece* Game::GetSelectedPiece() const {
    return m_selectedPiece;
}
CString Game::GetStatusMessage() const
{
    CString status;

    if (m_gameOver)
    {
        status.Format(_T("游戏结束！赢家是: %s"),
            m_winner == Camp::RED ? _T("红方") : _T("黑方"));
    }
    else
    {
        status.Format(_T("状态: %s   当前回合: %s"),
            _T("正常游戏中"),
            m_isRedTurn ? _T("红方") : _T("黑方"));
    }

    return status;
}