
// ChessDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Chess.h"
#include "ChessDlg.h"
#include "afxdialogex.h"
#include "Game.h"
#include "Board.h"
#include "NetworkManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChessDlg 对话框




CChessDlg::CChessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MessageEdit = _T("Display message");
	m_IpAddrEdit = _T("127.0.0.1");
}

void CChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_MessageEdit);
	DDX_Text(pDX, IDC_EDIT_IPADDR, m_IpAddrEdit);
}


BEGIN_MESSAGE_MAP(CChessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, &CChessDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_EXIT, &CChessDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CChessDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CChessDlg 消息处理程序

BOOL CChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		m_game.GetBoard().Draw(&dc);

		// 绘制状态信息
		CString status = m_game.GetStatusMessage();
		CRect rect;
		GetClientRect(&rect);
		rect.top = rect.bottom - 30;
		dc.SetBkMode(TRANSPARENT);
		// 设置文字颜色，比如红色（RGB(255, 0, 0)）
		dc.SetTextColor(RGB(255, 0, 0));
		dc.DrawText(status, rect, DT_LEFT | DT_VCENTER);
	}
}

void CChessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);

	// 棋盘起点和格子大小要和 Board::Draw 中一致
	const int cellSize = 60;
	const int offsetX = 30;
	const int offsetY = 30;

	int x = (point.x - offsetX + cellSize / 2) / cellSize;
	int y = (point.y - offsetY + cellSize / 2) / cellSize;

	if (x >= 0 && x < 9 && y >= 0 && y < 10)
	{
		m_game.OnClick(x, y);  // 处理选中或走子
		Invalidate();          // 请求重绘
	}
}

//设置计时器
// ChessDlg.cpp
void CChessDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		int fx, fy, tx, ty;
		if (m_net.IsConnected() && m_net.ReceiveMove(fx, fy, tx, ty))
		{
			m_game.ApplyMove(fx, fy, tx, ty, false);  // 被动方接收
			Invalidate();                             // 重绘
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChessDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_game = Game();                    // 重新创建游戏对象，重置所有状态
	m_game.SetNetworkManager(&m_net);  // 如果有联机功能，继续设置网络管理器

	Invalidate();
}

void CChessDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDOK);
}

void CChessDlg::OnBnClickedButtonConnect()
{
	CString strIP;
	CString strRole;
	// 1. 获取 IP 地址
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_IPADDR);
	pEdit->GetWindowText(strIP);
	// 2. 获取连接角色（服务器/客户端）
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	pCombo->GetWindowText(strRole);

	if (pCombo != NULL)
	{
		m_game.SetNetworkManager(&m_net);
		if (m_net.DynamicInitialize(strRole, strIP))
		{
			// 根据角色设置先手方：服务器为红方，客户端为黑方
			CString lowerRole = strRole;
			lowerRole.MakeLower();
			if (lowerRole == _T("server"))
			{
				m_game.SetRedTurn(true);  // 红方先手
			}
			else
			{
				m_game.SetRedTurn(false); // 黑方
			}

			//SetDlgItemText(IDC_EDIT_MESSAGE, _T("Connected"));
		}
		else
		{
			//SetDlgItemText(IDC_EDIT_MESSAGE, _T("Connect failed"));
		}
	}

}

