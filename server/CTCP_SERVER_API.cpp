#include "CTCP_SERVER_API.h"

CTCP_SERVER_API::CTCP_SERVER_API()
{

}

SOCKET CTCP_SERVER_API::Create_Socket(SOCKADDR_IN& addr_info, int port_add)
{
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening != INVALID_SOCKET)
    {
        memset(&addr_info, 0, sizeof(addr_info));
        addr_info.sin_family = AF_INET;
        addr_info.sin_port = htons(port + port_add);
        int bindOk = bind(listening, (struct sockaddr*)&addr_info, sizeof(addr_info));
        if (bindOk != SOCKET_ERROR)
        {
            int listenOk = listen(listening, SOMAXCONN);
            if (listenOk == SOCKET_ERROR)
            {
                std::cerr << "Don't listening --> Create_Socket method" << std::endl;
                return -1;
            }
        }
        else
        {
            std::cerr << "Didn't bind --> Create_Socket method" << std::endl;
            return -1;
        }
    }
    else
    {
        std::cerr << "Didnt create socket --> Create_Socket method" << std::endl;
        return -1;
    }

    return listening;
}

SOCKET CTCP_SERVER_API::Wait_For_Connection(SOCKET& s_listen, SOCKADDR_IN& addr_info)
{
    size_sockaddr size_addr = sizeof(addr_info);
    SOCKET client = accept(s_listen, (struct sockaddr*)&addr_info, &size_addr);
    return client;
}

bool CTCP_SERVER_API::close_sockets(SOCKET& s_listen, SOCKET& s_for_connection)
{
    int closeOk = closesocket(s_listen);
    if (closeOk != 0)
    {
        std::cerr << "Socket didn't close close_socket method" << std::endl;
        return false;
    }
    closeOk = closesocket(s_for_connection);
    if (closeOk != 0)
    {
        std::cerr << "Socket didn't close close_socket method" << std::endl;
        return false;
    }
    return true;
}

bool CTCP_SERVER_API::change_port(SOCKET& s_listen, SOCKET& s_for_connection, SOCKADDR_IN& addr_info,int &count)
{
    close_sockets(s_listen, s_for_connection);

    s_listen = Create_Socket(addr_info, count);
    if(s_listen == -1)
        exit(-1);

    s_for_connection = Wait_For_Connection(s_listen, addr_info);

    if (s_for_connection != 0) {
        std::cout << "\nCONNECT to port -> " << ntohs(addr_info.sin_port) << std::endl;
        return true;
    }

    return false;
}
