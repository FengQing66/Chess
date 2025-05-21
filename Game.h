#pragma once
#include "pch.h"
#include "Board.h"
#include "Piece.h"
#include <utility>
#include "NetworkManager.h"

class Game {
public:
    Game();
    void CheckGameOver();//检测游戏结束
    // 构造函数：初始化棋盘
    void OnClick(int x, int y);     // 响应点击事件
    bool IsValidCoord(int x, int y) const;
    const Piece* GetSelectedPiece() const;
    Board& GetBoard() { return m_board; }  // 返回 Board 引用
    const Board& GetBoard() const { return m_board; }
    //联机部分的函数
    void SetNetworkManager(CNetworkManager* net) { m_pNet = net; }
    void ApplyMove(int fromX, int fromY, int toX, int toY, bool isLocalMove = false);
    CString GetStatusMessage() const;
    bool m_gameOver = false;
    Camp m_winner = Camp::RED; // 默认
    void SetRedTurn(bool isRedTurn);//默认服务器红方

private:
    Board m_board;                  // 棋盘对象
    Piece* m_selectedPiece;         // 当前选中的棋子
    bool m_isRedTurn;               // 是否红方回合
    //联机
    CNetworkManager* m_pNet = nullptr;
};