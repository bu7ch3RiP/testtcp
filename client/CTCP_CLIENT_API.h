#ifndef CTCP_CLIENT_API_H_
#define CTCP_CLIENT_API_H_

#include "Include.h"

class CTCP_CLIENT_API
{
public:
	
	CTCP_CLIENT_API();
	 
	//Create Socket
	SOCKET Create_Socket();

	//Wait for connection
	//return false if didn't connect
	//return true if connected
	bool Create_Connection(int, SOCKET&, SOCKADDR_IN&);

	//Close sockets
	//return false if didn't close
	//else return true
	bool close_socket(SOCKET&);

	//Change port for connection
	//return false if didn't connect
	//else return true
	bool change_port(SOCKET&, SOCKADDR_IN& addr_info, int&);

};

#endif