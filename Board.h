#pragma once
#include "pch.h"
#include "Piece.h"
#include <afxwin.h>
#include <vector>
#include <utility>  // 为 std::pair

class Board {
public:
    Board();

    void Draw(CDC* pDC); // 绘制棋盘和棋子
    void ResetBoard();    // 初始化棋盘和棋子位置
    void UpdatePiecePosition(int fromX, int fromY, int toX, int toY);
    Piece* GetPieceAt(int x, int y);
    Piece* const (&GetBoardData() const)[10][9];
    void DrawPieces(CDC* pDC, const Piece* selectedPiece = nullptr);



private:
    Piece* m_board[10][9];            // 10行9列的棋盘指针数组
    std::vector<Piece> m_pieces;      // 存储所有棋子的对象

    void DrawBoard(CDC* pDC);
    void CreateInitialPieces();
};
