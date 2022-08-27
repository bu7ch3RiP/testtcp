#include "CTCP_CLIENT_API.h"

CTCP_CLIENT_API::CTCP_CLIENT_API()
{
}

SOCKET CTCP_CLIENT_API::Create_Socket()
{
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET)
	{
		std::cerr << "Socket didnt create --> Client-Create-Socket" << std::endl;
	}
	return client_socket;
}

bool CTCP_CLIENT_API::Create_Connection(int port_add, SOCKET& s_client, SOCKADDR_IN& addr_info)
{
	memset(&addr_info, 0, sizeof(addr_info));
	addr_info.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr_info.sin_family = AF_INET;
	addr_info.sin_port = htons(port + port_add);
	int size = sizeof(addr_info);
	int connectOk = connect(s_client, (sockaddr*)&addr_info, size);
	if (connectOk != 0)
	{
		closesocket(s_client);
		std::cerr << "Didn't connect --> Client::Create_Connection" << std::endl;
		return false;
	}
	return true;
}

bool CTCP_CLIENT_API::close_socket(SOCKET& s_client)
{
	int closeOk = closesocket(s_client);
	if (closeOk != 0)
	{
		std::cerr << "Socket didn't close" << std::endl;
		return false;
	}
	return true;
}

bool CTCP_CLIENT_API::change_port(SOCKET& s_client, SOCKADDR_IN& addr_info, int& count)
{
	memset(&addr_info, 0, sizeof(SOCKADDR_IN));
	addr_info.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr_info.sin_port = htons(8400 + count);
	addr_info.sin_family = AF_INET;

	s_client = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(s_client, (sockaddr*)&addr_info, sizeof(addr_info)) == 0) {
		std::cout << "\nCONNECT to port --> " << ntohs(addr_info.sin_port);
	}
	else
	{
		std::cout << "not connected" << std::endl;
		return false;
	}
	return true;
}
