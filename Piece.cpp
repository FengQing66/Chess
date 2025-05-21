#include "pch.h"
#include "Piece.h"
#include <cmath>


Piece::Piece(PieceType type, Camp camp, int x, int y)
    : m_type(type), m_camp(camp), m_x(x), m_y(y), m_alive(true) {
}

PieceType Piece::GetType() const {
    return m_type;
}

Camp Piece::GetCamp() const {
    return m_camp;
}

std::pair<int, int> Piece::GetPosition() const {
    return { m_x, m_y };
}

void Piece::SetPosition(int x, int y) {
    m_x = x;
    m_y = y;
}

bool Piece::IsAlive() const {
    return m_alive;
}

void Piece::Capture() {
    m_alive = false;
}
//是否合法走子
bool Piece::IsValidMove(int destX, int destY,   Piece* const (&board)[10][9]) const
{
    if (!m_alive) return false;

    if (destX < 0 || destX >= 9 || destY < 0 || destY >= 10)
        return false;

    const Piece* target = board[destY][destX];

    // 不能吃自己人
    if (target && target->GetCamp() == m_camp)
        return false;

    return CanMoveTo(destX, destY, board);
}
//走子逻辑
bool Piece::CanMoveTo(int destX, int destY,  Piece* const (&board)[10][9]) const
{
    int dx = destX - m_x;
    int dy = destY - m_y;

    switch (m_type)
    {
    case PieceType::GENERAL:
        if (destX < 3 || destX > 5) return false;
        if (m_camp == Camp::RED && (destY < 7 || destY > 9)) return false;
        if (m_camp == Camp::BLACK && (destY < 0 || destY > 2)) return false;
        return (abs(dx) + abs(dy) == 1); // 上下左右一格

    case PieceType::ADVISOR:
        if (destX < 3 || destX > 5) return false;
        if (m_camp == Camp::RED && (destY < 7 || destY > 9)) return false;
        if (m_camp == Camp::BLACK && (destY < 0 || destY > 2)) return false;
        return (abs(dx) == 1 && abs(dy) == 1); // 对角线一格

    case PieceType::ELEPHANT:
        if (m_camp == Camp::RED && destY < 5) return false;
        if (m_camp == Camp::BLACK && destY > 4) return false;
        if (abs(dx) != 2 || abs(dy) != 2) return false;
        return board[m_y + dy / 2][m_x + dx / 2] == nullptr; // 象眼不能被堵

    case PieceType::HORSE:
        if (!((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2))) return false;
        if (abs(dx) == 2)
            return board[m_y][m_x + dx / 2] == nullptr; // 横蹩马腿
        else
            return board[m_y + dy / 2][m_x] == nullptr; // 竖蹩马腿

    case PieceType::CHARIOT:
    {
        if (dx != 0 && dy != 0) return false;
        if (dx == 0)
        {
            int step = (dy > 0 ? 1 : -1);
            for (int y = m_y + step; y != destY; y += step)
                if (board[y][m_x]) return false;
        }
        else
        {
            int step = (dx > 0 ? 1 : -1);
            for (int x = m_x + step; x != destX; x += step)
                if (board[m_y][x]) return false;
        }
        return true;
    }

    case PieceType::CANNON:
    {
        if (dx != 0 && dy != 0) return false;
        int count = 0;
        if (dx == 0)
        {
            int step = (dy > 0 ? 1 : -1);
            for (int y = m_y + step; y != destY; y += step)
                if (board[y][m_x]) ++count;
        }
        else
        {
            int step = (dx > 0 ? 1 : -1);
            for (int x = m_x + step; x != destX; x += step)
                if (board[m_y][x]) ++count;
        }
        // 炮打空目标时必须无阻挡；打人时必须隔一个
        return (board[destY][destX] == nullptr && count == 0) ||
            (board[destY][destX] != nullptr && count == 1);
    }

    case PieceType::SOLDIER:
        if (m_camp == Camp::RED)
        {
            if (m_y >= 5)  // 未过河
                return dx == 0 && dy == -1;
            else
                return (dx == 0 && dy == -1) || (dy == 0 && abs(dx) == 1);
        }
        else
        {
            if (m_y <= 4)  // 未过河
                return dx == 0 && dy == 1;
            else
                return (dx == 0 && dy == 1) || (dy == 0 && abs(dx) == 1);
        }

    default:
        return false;
    }
}
