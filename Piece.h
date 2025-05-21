#pragma once
#include "pch.h"
#include <string>
#include <utility>

enum class PieceType {
    GENERAL, ADVISOR, ELEPHANT, HORSE, CHARIOT, CANNON, SOLDIER, NONE
};

enum class Camp {
    RED, BLACK, NONE
};

class Piece {
public:
    Piece(PieceType type = PieceType::NONE, Camp camp = Camp::NONE, int x = -1, int y = -1);

    PieceType GetType() const;
    Camp GetCamp() const;
    std::pair<int, int> GetPosition() const;

    void SetPosition(int x, int y);
    bool IsAlive() const;
    void Capture();
    bool CanMoveTo(int destX, int destY,  Piece* const (&board)[10][9]) const;

    bool IsValidMove(int destX, int destY,  Piece* const (&board)[10][9]) const;



private:
    PieceType m_type;
    Camp m_camp;
    int m_x;
    int m_y;
    bool m_alive;

    
};
