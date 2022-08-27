#ifndef Server_H_
#define Server_H_

#include "General.h"
#include "CTCP_SERVER_API.h"


class Server : public CTCP_SERVER_API
{
private:

	//fields
#ifdef _WIN32
	WORD dllVer;
	WSAData wsad;
#endif

	SOCKADDR_IN addr_info;
	SOCKET s_listen;
	SOCKET s_for_connection;

	std::string path;
	std::vector<std::string> client_file_list;

	int RecvBuffer(SOCKET& s, char* buffer, int bufferSize, int chunkSize = Chunk_Buffer_Size);


	// returns the path to the folder in which the project is located
	std::string get_path_of_executable_program();

	// Receives a file
	// returns size of file if success
	// returns -1 if file couldn't be opened for output
	// returns -2 if couldn't receive file length properly
	// returns -3 if couldn't receive file properly
	int64_t RecvFile(SOCKET& s, const std::string& fileName, int chunkSize = Chunk_RecFile_Size);

public:
	
	Server();
	~Server();

	//return true if list of files received 
	//return false if list of files is empty
	bool get_list_of_files_from_client(SOCKET&);


	// Send to the client the file name to download it
	// and return this file name
	std::string send_filename_to_transfer(SOCKET&, int);


	// Start receiving files
	void Do_work(SOCKET&, std::string&);


	// return size of client_file_list
	int get_client_file_list_size() const;


	void Run();

};


#endif