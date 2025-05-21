
// ChessDlg.h: 头文件
//

#pragma once
#include "Piece.h"
#include "Board.h"
#include "Game.h"

// CChessDlg 对话框
class CChessDlg : public CDialogEx
{
// 构造
public:
	CChessDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CString m_MessageEdit; // 关联Message编辑框
	CString m_IpAddrEdit;  // 关联IP_Addr编辑框

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	Game m_game;
	CNetworkManager m_net;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//左键鼠标
	void OnTimer(UINT_PTR nIDEvent);//计时器
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedButtonConnect();
};
