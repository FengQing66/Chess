#pragma once
#include "pch.h"
#include "Piece.h"
#include <afxwin.h>
#include <vector>
#include <utility>  // Ϊ std::pair

class Board {
public:
    Board();

    void Draw(CDC* pDC); // �������̺�����
    void ResetBoard();    // ��ʼ�����̺�����λ��
    void UpdatePiecePosition(int fromX, int fromY, int toX, int toY);
    Piece* GetPieceAt(int x, int y);
    Piece* const (&GetBoardData() const)[10][9];
    void DrawPieces(CDC* pDC, const Piece* selectedPiece = nullptr);



private:
    Piece* m_board[10][9];            // 10��9�е�����ָ������
    std::vector<Piece> m_pieces;      // �洢�������ӵĶ���

    void DrawBoard(CDC* pDC);
    void CreateInitialPieces();
};
