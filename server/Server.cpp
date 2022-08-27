#include "Server.h"

int Server::RecvBuffer(SOCKET& s, char* buffer, int bufferSize, int chunkSize)
{
    int i = 0;
    while (i < bufferSize) {
        const int l = recv(s, &buffer[i], __min(chunkSize, bufferSize - i), 0);
        if (l < 0) { return l; } // this is an error
        i += l;
    }
    return i;
}

std::string Server::get_path_of_executable_program()
{
    std::filesystem::path temp_path = std::filesystem::current_path();
    std::string str = temp_path.string();
    return str;
}

int64_t Server::RecvFile(SOCKET& s, const std::string& fileName, int chunkSize)
{
    std::ofstream file(fileName, std::ofstream::binary);
    if (file.fail()) { return -1; }

    int64_t fileSize;
    if (RecvBuffer(s, reinterpret_cast<char*>(&fileSize), sizeof(fileSize)) != sizeof(fileSize))
    {
        return -2;
    }

    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t i = fileSize;
    int64_t checker = 0;
    int sum_of_transfer = 0;
    int procent = 10;
    int add_port_number = 5;
    while (i != 0) {
        const int r = RecvBuffer(s, buffer, (int)__min(i, (int64_t)chunkSize));

        sum_of_transfer += r;
        checker = fileSize * procent / 100;

        if (sum_of_transfer >= checker)
        {
            std::cout << "Procents: " << procent << "\t--> " << sum_of_transfer << std::endl;
            procent += 10;
            change_port(s_listen, s_for_connection, addr_info, add_port_number);
            add_port_number += 5;
        }
        if ((r < 0) || !file.write(buffer, r)) { errored = true; break; }
        i -= r;

    }
    delete[] buffer;

    file.close();

    return errored ? -3 : fileSize;
}

Server::Server()
{
#ifdef _WIN32
    dllVer = MAKEWORD(2, 1);
    int wsInit = WSAStartup(dllVer, &wsad);
#endif

    Run();
}

Server::~Server()
{
    close_sockets(this->s_listen, this->s_for_connection);
}

bool Server::get_list_of_files_from_client(SOCKET& s)
{
    //get size of vector<string> in which located all files of the client folder
    int msg_size;
    recv(s, (char*)&msg_size, sizeof(int), 0);
    char* msg = new char[msg_size + 1];
    msg[msg_size] = '\0';
    recv(s, msg, msg_size, 0);
    int counter = atoi(msg);
    delete[] msg;
    if (counter <= 0)
    {
        std::cerr << "Didn't take the file size from client." << std::endl;
        return false;
    }

    //save the list of data files on the server
    for (int i = 0; i < counter; ++i)
    {
        recv(s, (char*)&msg_size, sizeof(int), 0);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(s, msg, msg_size, 0);

        this->client_file_list.push_back(msg);
        delete[] msg;
    }

    // print the list of data files
    for (int i = 0; i < counter; ++i)
    {
        std::cout << this->client_file_list[i] << std::endl;
    }


    return true;
}

std::string Server::send_filename_to_transfer(SOCKET& s, int count)
{
    int msg_size = client_file_list[count].size();
    send(s, (const char*)&msg_size, sizeof(int), 0);

    send(s, (const char*)this->client_file_list[count].c_str(), msg_size, 0);

    std::string temp = this->client_file_list[count];
    std::string t_p = "";

    for (int i = temp.size(); i > 0; --i)
    {

#ifdef _WIN32
        if (temp[i] == '\\')
            break;
#elif __linux__
        if (temp[i] == '/')
            break;
#endif
        t_p += temp[i];
    }

    std::reverse(t_p.begin(), t_p.end());
    return t_p;
}

void Server::Do_work(SOCKET& s , std::string& temp_path)
{
    RecvFile(s, path + temp_path);
}

int Server::get_client_file_list_size() const
{
    return this->client_file_list.size();
}

void Server::Run()
{
    s_listen = Create_Socket(addr_info, 0);
    if (s_listen == INVALID_SOCKET) {
        std::cerr << "Socket didnt create --> Run method" << std::endl;
        exit(1);
    }

    s_for_connection = Wait_For_Connection(s_listen, addr_info);
    if (s_for_connection == INVALID_SOCKET) {
        std::cerr << "Didnt connect" << std::endl;
        exit(1);
    }

    if (!get_list_of_files_from_client(s_for_connection)) {
        std::cerr << "File list is empty." << std::endl;
        exit(-1);
    }

    const int size = get_client_file_list_size();

    std::string tmp_path;
    for (int i = 0; i < size; ++i) {
        tmp_path = send_filename_to_transfer(s_for_connection, i);
        Do_work(s_for_connection, tmp_path);
    }
}
