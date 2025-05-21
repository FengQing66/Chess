#pragma once  
class CNetworkManager
{
public:
    CNetworkManager();
    virtual ~CNetworkManager();

    enum ConnectionRole { SERVER, CLIENT };

    void CloseConnection();
    BOOL DynamicInitialize(LPCTSTR lpszRoleOrCommand, LPCTSTR lpszServerIP);

    //��piece����������
    BOOL SendMove(int fromX, int fromY, int toX, int toY);
    BOOL ReceiveMove(int& fromX, int& fromY, int& toX, int& toY);


    BOOL IsConnected() const { return m_bConnected; }

private:
    SOCKET m_socket;
    SOCKET m_clientSocket; // ���ڷ�������
    BOOL m_bConnected;
    ConnectionRole m_role;

    BOOL StartServer(LPCTSTR lpszServerIP);
    BOOL ConnectToServer(LPCTSTR lpszServerIP);
};
