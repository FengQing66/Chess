#pragma once  
class CNetworkManager
{
public:
    CNetworkManager();
    virtual ~CNetworkManager();

    enum ConnectionRole { SERVER, CLIENT };

    void CloseConnection();
    BOOL DynamicInitialize(LPCTSTR lpszRoleOrCommand, LPCTSTR lpszServerIP);

    //从piece类里掏数据
    BOOL SendMove(int fromX, int fromY, int toX, int toY);
    BOOL ReceiveMove(int& fromX, int& fromY, int& toX, int& toY);


    BOOL IsConnected() const { return m_bConnected; }

private:
    SOCKET m_socket;
    SOCKET m_clientSocket; // 用于服务器端
    BOOL m_bConnected;
    ConnectionRole m_role;

    BOOL StartServer(LPCTSTR lpszServerIP);
    BOOL ConnectToServer(LPCTSTR lpszServerIP);
};
