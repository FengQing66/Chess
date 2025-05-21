#include "pch.h"
#include "NetworkManager.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

CNetworkManager::CNetworkManager()
    : m_socket(INVALID_SOCKET), m_clientSocket(INVALID_SOCKET), m_bConnected(FALSE), m_role(SERVER)
{
}

CNetworkManager::~CNetworkManager()
{
    CloseConnection();
}

// ������������
BOOL CNetworkManager::StartServer(LPCTSTR lpszServerIP)
{
    // 1. ����Socket
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        AfxMessageBox(_T("Failed to create server socket!"));
        return FALSE;
    }
   // AfxMessageBox(_T("Server socket created in server ip successfully."));

    // 2. �󶨵�ַ�Ͷ˿�
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    if (InetPton(AF_INET, lpszServerIP, &addr.sin_addr) != 1) {
        AfxMessageBox(_T("Invalid IP address format!"));
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return FALSE;
    }

    if (bind(m_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        AfxMessageBox(_T("Failed to bind socket!"));
        return FALSE;
    }
    //AfxMessageBox(_T("Socket bound to port 8888."));

    // 3. ��ʼ����
    if (listen(m_socket, 1) == SOCKET_ERROR) {
        AfxMessageBox(_T("Failed to listen on socket!"));
        return FALSE;
    }
    //AfxMessageBox(_T("Server is listening for connections..."));

    // 4. �ȴ��ͻ������ӣ���ʱ3�����˳�

    // ������ socket ����Ϊ������
    u_long mode = 1;
    ioctlsocket(m_socket, FIONBIO, &mode);

    // ʹ�� select �ȴ��ͻ������ӣ����ȴ� 3 ����
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(m_socket, &readfds);

    timeval timeout;
    timeout.tv_sec = 8;  //�ȴ�ʱ��
    timeout.tv_usec = 0;

    int sel = select(0, &readfds, nullptr, nullptr, &timeout);
    if (sel <= 0) {
        CString msg;
        msg.Format(_T("No client connected within 3 minutes (select error: %d)."), WSAGetLastError());
        AfxMessageBox(msg);
        return FALSE;
    }
    // �пͻ����������󣬵��� accept()
    sockaddr_in clientAddr = {};
    int len = sizeof(clientAddr);
    m_clientSocket = accept(m_socket, (SOCKADDR*)&clientAddr, &len);
    if (m_clientSocket == INVALID_SOCKET) {
        CString msg;
        msg.Format(_T("Failed to accept client connection! Error: %d"), WSAGetLastError());
        AfxMessageBox(msg);
        return FALSE;
    }
    AfxMessageBox(_T("Client connected successfully!"));
    return TRUE;
}

// ��Ϊ�ͻ������ӷ�����
BOOL CNetworkManager::ConnectToServer(LPCTSTR lpszServerIP)
{
    CloseConnection();

    // ȷ�� WSA ��ʼ��
    WSADATA wsaData;
    int wsaInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaInit != 0) {
        CString msg;
        msg.Format(_T("WSAStartup failed: %d"), wsaInit);
        AfxMessageBox(msg);
        return FALSE;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        int err = WSAGetLastError();
        CString msg;
        msg.Format(_T("Failed to create client socket! WSA error: %d"), err);
        AfxMessageBox(msg);
        return FALSE;
    }

    // 2. ���÷�������ַ
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    if (inet_pton(AF_INET, CT2A(lpszServerIP), &serverAddr.sin_addr) <= 0) {
        AfxMessageBox(_T("Invalid server IP address!"));
        return FALSE;
    }
    AfxMessageBox(_T("Server address set."));

    // 3. ���ӷ�����
    if (connect(m_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        CString msg;
        msg.Format(_T("connect() failed with error: %d"), WSAGetLastError());
        AfxMessageBox(msg);
        return FALSE;
    }
    

    m_bConnected = TRUE;
    AfxMessageBox(_T("Connected to server successfully!"));

    return TRUE;
}

// �ر�����
void CNetworkManager::CloseConnection()
{
    if (m_clientSocket != INVALID_SOCKET) {
        closesocket(m_clientSocket);
        m_clientSocket = INVALID_SOCKET;
        AfxMessageBox(_T("Client socket closed."));
    }

    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        AfxMessageBox(_T("Main socket closed."));
    }

    WSACleanup();
    m_bConnected = FALSE;
   // AfxMessageBox(_T("Winsock resources cleaned up."));
}

// ������������
BOOL CNetworkManager::SendMove(int fromX, int fromY, int toX, int toY)
{
    int data[4] = { fromX, fromY, toX, toY };
    SOCKET s = (m_role == SERVER) ? m_clientSocket : m_socket;

    if (send(s, (char*)data, sizeof(data), 0) != sizeof(data)) {
       // AfxMessageBox(_T("Failed to send move data!"));
        return FALSE;
    }
   // AfxMessageBox(_T("Move data sent successfully."));
    return TRUE;
}

// ������������
BOOL CNetworkManager::ReceiveMove(int& fromX, int& fromY, int& toX, int& toY)
{
    int data[4] = {};
    SOCKET s = (m_role == SERVER) ? m_clientSocket : m_socket;

    int received = recv(s, (char*)data, sizeof(data), 0);
    if (received != sizeof(data)) {
       // AfxMessageBox(_T("Failed to receive move data!"));
        return FALSE;
    }

    fromX = data[0];
    fromY = data[1];
    toX = data[2];
    toY = data[3];
    //AfxMessageBox(_T("Move data received successfully."));
    return TRUE;
}

// ��̬��ʼ����ɫ��Server/Client��
BOOL CNetworkManager::DynamicInitialize(LPCTSTR lpszRoleOrCommand, LPCTSTR lpszServerIP)
{
    CloseConnection();

    CString roleStr = lpszRoleOrCommand ? lpszRoleOrCommand : _T("");
    roleStr.MakeLower();

    if (roleStr == _T("server")) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            AfxMessageBox(_T("WSAStartup failed!"));
            return FALSE;
        }
        AfxMessageBox(_T("Winsock initialized successfully."));

        AfxMessageBox(_T("Starting as SERVER on port 8888..."));
        return StartServer(lpszServerIP);
    }
    else if (roleStr == _T("client")) {
        AfxMessageBox(_T("Starting as CLIENT, connecting to server..."));

        return ConnectToServer(lpszServerIP);
    }

    AfxMessageBox(_T("Invalid role. Must be 'server' or 'client'."));
    return FALSE;
}