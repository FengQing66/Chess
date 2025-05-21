#include "pch.h"
#include "Board.h"
#include "Piece.h"
#include <utility>  // 为 std::pair

Board::Board() {
    ResetBoard();
}

void Board::ResetBoard() {
    // 清空指针表和对象表
    m_pieces.clear();
    for (int y = 0; y < 10; ++y)
        for (int x = 0; x < 9; ++x)
            m_board[y][x] = nullptr;

    CreateInitialPieces();

    // 填充 m_board
    for (Piece& piece : m_pieces) {
        std::pair<int, int> pos = piece.GetPosition();
        int x = pos.first;
        int y = pos.second;

        m_board[y][x] = &piece;
    }
}

void Board::CreateInitialPieces() {
    // 红方部分
    m_pieces.emplace_back(PieceType::GENERAL, Camp::RED, 4, 9);
    m_pieces.emplace_back(PieceType::ADVISOR, Camp::RED, 3, 9);
    m_pieces.emplace_back(PieceType::ADVISOR, Camp::RED, 5, 9);
    m_pieces.emplace_back(PieceType::ELEPHANT, Camp::RED, 2, 9);  // 红方相
    m_pieces.emplace_back(PieceType::ELEPHANT, Camp::RED, 6, 9);
    m_pieces.emplace_back(PieceType::HORSE, Camp::RED, 1, 9);    // 红方马
    m_pieces.emplace_back(PieceType::HORSE, Camp::RED, 7, 9);
    m_pieces.emplace_back(PieceType::CHARIOT, Camp::RED, 0, 9);  // 红方车
    m_pieces.emplace_back(PieceType::CHARIOT, Camp::RED, 8, 9);
    m_pieces.emplace_back(PieceType::CANNON, Camp::RED, 1, 7);    // 红方炮
    m_pieces.emplace_back(PieceType::CANNON, Camp::RED, 7, 7);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::RED, 0, 6);   // 红方兵
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::RED, 2, 6);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::RED, 4, 6);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::RED, 6, 6);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::RED, 8, 6);

    // 黑方部分
    m_pieces.emplace_back(PieceType::GENERAL, Camp::BLACK, 4, 0);
    m_pieces.emplace_back(PieceType::ADVISOR, Camp::BLACK, 3, 0);
    m_pieces.emplace_back(PieceType::ADVISOR, Camp::BLACK, 5, 0);
    m_pieces.emplace_back(PieceType::ELEPHANT, Camp::BLACK, 2, 0); // 黑方象
    m_pieces.emplace_back(PieceType::ELEPHANT, Camp::BLACK, 6, 0);
    m_pieces.emplace_back(PieceType::HORSE, Camp::BLACK, 1, 0);   // 黑方马
    m_pieces.emplace_back(PieceType::HORSE, Camp::BLACK, 7, 0);
    m_pieces.emplace_back(PieceType::CHARIOT, Camp::BLACK, 0, 0); // 黑方车
    m_pieces.emplace_back(PieceType::CHARIOT, Camp::BLACK, 8, 0);
    m_pieces.emplace_back(PieceType::CANNON, Camp::BLACK, 1, 2);  // 黑方炮
    m_pieces.emplace_back(PieceType::CANNON, Camp::BLACK, 7, 2);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::BLACK, 0, 3); // 黑方卒
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::BLACK, 2, 3);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::BLACK, 4, 3);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::BLACK, 6, 3);
    m_pieces.emplace_back(PieceType::SOLDIER, Camp::BLACK, 8, 3);
}

void Board::Draw(CDC* pDC) {
    DrawBoard(pDC);
    DrawPieces(pDC);
}

void Board::DrawBoard(CDC* pDC) {
    const int cellSize = 60;
    const int offsetX = 30, offsetY = 30;

    // 设置棋盘背景色为浅木色
    CBrush brush(RGB(240, 210, 160));
    CBrush* oldBrush = pDC->SelectObject(&brush);
    CRect boardRect(offsetX, offsetY, offsetX + 8 * cellSize, offsetY + 9 * cellSize);
    pDC->FillRect(&boardRect, &brush);
    pDC->SelectObject(oldBrush);

    // 创建一个较粗的画笔，宽度设为 2，颜色为深棕色，用于绘制棋盘线条
    CPen pen(PS_SOLID, 2, RGB(139, 69, 19));
    CPen* oldPen = pDC->SelectObject(&pen);

    for (int i = 0; i < 10; ++i) {
        pDC->MoveTo(offsetX, offsetY + i * cellSize);
        pDC->LineTo(offsetX + 8 * cellSize, offsetY + i * cellSize);
    }
    for (int j = 0; j < 9; ++j) {
        pDC->MoveTo(offsetX + j * cellSize, offsetY);
        pDC->LineTo(offsetX + j * cellSize, offsetY + 9 * cellSize);
    }

    // 绘制楚河汉界，增大间距
    CString riverText1 = _T("楚河");
    CString riverText2 = _T("漢界");
    CRect riverRect1(offsetX + 1.5 * cellSize, offsetY + 4 * cellSize,
        offsetX + 3.5 * cellSize, offsetY + 5 * cellSize);
    CRect riverRect2(offsetX + 4.5 * cellSize, offsetY + 4 * cellSize,
        offsetX + 6.5 * cellSize, offsetY + 5 * cellSize);

    // 设置字体
    CFont font;
    font.CreatePointFont(240, _T("隶书"), pDC);
    CFont* oldFont = pDC->SelectObject(&font);

    // 设置文本颜色为深棕色
    pDC->SetTextColor(RGB(139, 69, 19));
    pDC->SetBkMode(TRANSPARENT);

    // 绘制文本
    pDC->DrawText(riverText1, riverRect1, DT_CENTER | DT_VCENTER);
    pDC->DrawText(riverText2, riverRect2, DT_CENTER | DT_VCENTER);

    // 绘制九宫格斜线（米字格）
    // 红方九宫格
    pDC->MoveTo(offsetX + 3 * cellSize, offsetY + 9 * cellSize);
    pDC->LineTo(offsetX + 5 * cellSize, offsetY + 7 * cellSize);
    pDC->MoveTo(offsetX + 5 * cellSize, offsetY + 9 * cellSize);
    pDC->LineTo(offsetX + 3 * cellSize, offsetY + 7 * cellSize);
    // 黑方九宫格
    pDC->MoveTo(offsetX + 3 * cellSize, offsetY);
    pDC->LineTo(offsetX + 5 * cellSize, offsetY + 2 * cellSize);
    pDC->MoveTo(offsetX + 5 * cellSize, offsetY);
    pDC->LineTo(offsetX + 3 * cellSize, offsetY + 2 * cellSize);

    // 恢复字体和画笔
    pDC->SelectObject(oldFont);
    pDC->SelectObject(oldPen);
}

void Board::DrawPieces(CDC* pDC, const Piece* selectedPiece)
{
    const int cellSize = 60;
    const int offsetX = 30, offsetY = 30;

    // 创建一个较粗的画笔，宽度设为 2，默认颜色（选中时为红色，未选中时为黑色）
    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* oldPen = pDC->SelectObject(&pen);

    for (const Piece& piece : m_pieces) {
        if (!piece.IsAlive()) continue;
        auto [x, y] = piece.GetPosition();

        CRect rect(
            offsetX + x * cellSize - 20,
            offsetY + y * cellSize - 20,
            offsetX + x * cellSize + 20,
            offsetY + y * cellSize + 20
        );

        // 根据棋子阵营设置不同的背景颜色
        CBrush brush(piece.GetCamp() == Camp::RED ?
            RGB(255, 255, 255) : RGB(30, 30, 30));
        CBrush* oldBrush = pDC->SelectObject(&brush);

        // 高亮选中棋子，边框设为红色
        if (&piece == selectedPiece) {
            CPen selectPen(PS_SOLID, 2, RGB(255, 0, 0));
            CPen* oldSelectPen = pDC->SelectObject(&selectPen);
            pDC->Ellipse(&rect);
            pDC->SelectObject(oldSelectPen);
        }
        else {
            pDC->Ellipse(&rect);
        }

        pDC->SelectObject(oldBrush);

        // 根据棋子阵营设置不同的文字颜色
        COLORREF textColor = piece.GetCamp() == Camp::RED ?
            RGB(255, 0, 0) : RGB(255, 255, 255);
        pDC->SetTextColor(textColor);

        // 棋子名称
        CString name = _T("?");
        switch (piece.GetType()) {
        case PieceType::GENERAL: name = (piece.GetCamp() == Camp::RED) ? _T("帥") : _T("將"); break;
        case PieceType::ADVISOR: name = (piece.GetCamp() == Camp::RED) ? _T("仕") : _T("士"); break;
        case PieceType::ELEPHANT: name = (piece.GetCamp() == Camp::RED) ? _T("相") : _T("象"); break;
        case PieceType::HORSE: name = _T("馬"); break;
        case PieceType::CHARIOT: name = _T("車"); break;
        case PieceType::CANNON: name = _T("炮"); break;
        case PieceType::SOLDIER: name = (piece.GetCamp() == Camp::RED) ? _T("兵") : _T("卒"); break;
        default: break;
        }

        pDC->DrawText(name, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    pDC->SelectObject(oldPen);
}

Piece* Board::GetPieceAt(int x, int y) {
    if (x < 0 || x >= 9 || y < 0 || y >= 10) return nullptr;
    return m_board[y][x];
}

void Board::UpdatePiecePosition(int fromX, int fromY, int toX, int toY) {
    Piece* piece = GetPieceAt(fromX, fromY);
    if (!piece) return;

    Piece* target = GetPieceAt(toX, toY);
    if (target) target->Capture();

    piece->SetPosition(toX, toY);
    m_board[toY][toX] = piece;
    m_board[fromY][fromX] = nullptr;
}

Piece* const (&Board::GetBoardData() const)[10][9]
{
    return m_board;
}
