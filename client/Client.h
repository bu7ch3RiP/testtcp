#ifndef Client_H_
#define Client_H_

#include "Include.h"
#include "CTCP_CLIENT_API.h"

class Client : public CTCP_CLIENT_API
{
private:
	//fields
#ifdef _WIN32
	WORD dllVer;
	WSAData wsad;
#endif

	SOCKADDR_IN addr_info;

	SOCKET s_client;


	std::vector<std::string> transfer_list;

	// return file size
	int64_t GetFileSize(const std::string& fileName);

	// Sends data in buffer until bufferSize value is met
	int SendBuffer(SOCKET& s, const char* buffer, int bufferSize, int chunkSize = Chunk_Buffer_Size);

	// Sends a file
	// returns size of file if success
	// returns -1 if file couldn't be opened for input
	// returns -2 if couldn't send file length properly
	// returns -3 if file couldn't be sent properly
	int64_t SendFile(SOCKET& s, const std::string& fileName, int chunkSize = Chunk_RecFile_Size);

public:

	Client();
	~Client();

	void Run();

	// Start sending files
	void Do_work(SOCKET&, std::string&);


	// Get list of files in directory
	// which client choose
	// return true if size of list bigger then 0
	// else return false
	bool get_list_of_files();


	// Send list of files to the server
	// return true if the list size bigger then 0
	// else return false
	bool send_list_of_files(SOCKET&);


	// Return the filename to upload the file to the server
	std::string get_filename_to_upload(SOCKET&);

	// Return size of transfer_list
	int get_transfer_list_size() const;

};

#endif