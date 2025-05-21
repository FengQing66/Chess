#pragma once
#include "pch.h"
#include "Board.h"
#include "Piece.h"
#include <utility>
#include "NetworkManager.h"

class Game {
public:
    Game();
    void CheckGameOver();//�����Ϸ����
    // ���캯������ʼ������
    void OnClick(int x, int y);     // ��Ӧ����¼�
    bool IsValidCoord(int x, int y) const;
    const Piece* GetSelectedPiece() const;
    Board& GetBoard() { return m_board; }  // ���� Board ����
    const Board& GetBoard() const { return m_board; }
    //�������ֵĺ���
    void SetNetworkManager(CNetworkManager* net) { m_pNet = net; }
    void ApplyMove(int fromX, int fromY, int toX, int toY, bool isLocalMove = false);
    CString GetStatusMessage() const;
    bool m_gameOver = false;
    Camp m_winner = Camp::RED; // Ĭ��
    void SetRedTurn(bool isRedTurn);//Ĭ�Ϸ������췽

private:
    Board m_board;                  // ���̶���
    Piece* m_selectedPiece;         // ��ǰѡ�е�����
    bool m_isRedTurn;               // �Ƿ�췽�غ�
    //����
    CNetworkManager* m_pNet = nullptr;
};