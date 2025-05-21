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
        // ѡ�м�������
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

            // ������������ ApplyMove �����Ϊ���ط���
            ApplyMove(from.first, from.second, x, y, true);
        }
    }
}

//��һ����ս�Ľ���������Ϣ
void Game::ApplyMove(int fromX, int fromY, int toX, int toY , bool isLocalMove)
{
    if (m_gameOver) return;

    Piece* moving = m_board.GetPieceAt(fromX, fromY);
    Piece* target = m_board.GetPieceAt(toX, toY);

    if (!moving || !moving->IsAlive()) return;

    // �����߼�
    if (target && target->GetCamp() != moving->GetCamp()) {
        target->Capture();
    }

    // ��������
    m_board.UpdatePiecePosition(fromX, fromY, toX, toY);

    // ����Ǳ���������ӣ����͸��Է�
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
        status.Format(_T("��Ϸ������Ӯ����: %s"),
            m_winner == Camp::RED ? _T("�췽") : _T("�ڷ�"));
    }
    else
    {
        status.Format(_T("״̬: %s   ��ǰ�غ�: %s"),
            _T("������Ϸ��"),
            m_isRedTurn ? _T("�췽") : _T("�ڷ�"));
    }

    return status;
}