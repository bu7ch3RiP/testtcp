#ifndef CTCP_SERVER_API_H_
#define CTCP_SERVER_API_H_

#include "General.h"

class CTCP_SERVER_API
{
public:

	CTCP_SERVER_API();

	//Create Socket
	SOCKET Create_Socket(SOCKADDR_IN& addr_info, int port_add);

	//Wait for client connection
	SOCKET Wait_For_Connection(SOCKET&, SOCKADDR_IN&);

	//Close sockets
	//return false if didn't close
	//else return true
	bool close_sockets(SOCKET&, SOCKET&);


	//Change port for connection
	//return false if didn't connect
	//else return true
	bool change_port(SOCKET&, SOCKET&, SOCKADDR_IN& addr_info, int&);
};


#endif