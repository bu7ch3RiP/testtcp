#include "Client.h"

int64_t Client::GetFileSize(const std::string& fileName)
{
    std::ifstream mySource;
    mySource.open(fileName, std::ios_base::binary);
    mySource.seekg(0, std::ios_base::end);
    int size = mySource.tellg();
    mySource.close();
    return size;
}

int Client::SendBuffer(SOCKET& s, const char* buffer, int bufferSize, int chunkSize)
{
    int i = 0;
    while (i < bufferSize) {
        const int l = send(s, &buffer[i], __min(chunkSize, bufferSize - i), 0);
        if (l < 0) { return l; } // this is an error
        i += l;
    }
    return i;
}

int64_t Client::SendFile(SOCKET& s, const std::string& fileName, int chunkSize)
{
    const int64_t fileSize = GetFileSize(fileName);
    if (fileSize < 0) { return -1; }

    std::ifstream file(fileName, std::ifstream::binary);
    if (file.fail()) { return -1; }

    if (SendBuffer(s, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize)) != sizeof(fileSize)) {
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
        const int64_t ssize = __min(i, (int64_t)chunkSize);
        if (!file.read(buffer, ssize)) { errored = true; break; }
        const int l = SendBuffer(s, buffer, (int)ssize);
        sum_of_transfer += l;
        checker = fileSize * procent / 100;
        if (sum_of_transfer >= checker)
        {
            std::cout << "Procents: " << procent << "\t--> " << sum_of_transfer << std::endl;
            procent += 10;
            Sleep(30);
            change_port(s, addr_info, add_port_number);
            add_port_number += 5;
        }

        if (l < 0) { errored = true; break; }
        i -= l;
    }
    delete[] buffer;

    file.close();

    return errored ? -3 : fileSize;
}

Client::Client()
{
#ifdef _WIN32
    dllVer = MAKEWORD(2, 1);
    WSAStartup(dllVer, &wsad);
#endif

    Run();
}

Client::~Client()
{
    close_socket(s_client);
}

void Client::Run()
{
    this->s_client = Create_Socket();
    if (this->Create_Connection(0, s_client, addr_info))
    {
        if (!get_list_of_files())
        {
            std::cerr << "Didn't get list of files" << std::endl;
            exit(-1);
        }

        if (!send_list_of_files(s_client))
        {
            std::cerr << "Didn't send list of files" << std::endl;
            exit(-1);
        }

        int size = get_transfer_list_size();

        for (int i = 0; i < size; ++i)
        {
            std::string t_path = get_filename_to_upload(s_client);
            Do_work(s_client, t_path);
        }
    }
}

void Client::Do_work(SOCKET& s, std::string& path)
{
    SendFile(s, path);
}

bool Client::get_list_of_files()
{
    std::cout << "Enter the pass to your folder: ";
    bool check = true;
    std::string path;

    while (check) {

        std::cin >> path;

        if (std::filesystem::exists(path))
            check = false;
        else
            std::cout << "This folder is not exist try again :)\n";
    }

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_regular_file(entry)) {
            if (GetFileSize(entry.path().string()) > size_100mb) {
                transfer_list.push_back(entry.path().string());
            }
        }
    }

    if (transfer_list.size() <= 0)
        return false;

    return true;
}

bool Client::send_list_of_files(SOCKET& s)
{
    int msg_size;
    std::string sttr = std::to_string(transfer_list.size());
    msg_size = sttr.size();
    send(s, (const char*)&msg_size, sizeof(int), 0);
    send(s, (const char*)sttr.c_str(), msg_size, 0);

    if (msg_size <= 0)
        return false;


    for (int i = 0; i < transfer_list.size(); ++i)
    {
        msg_size = transfer_list[i].size();
        send(s, (const char*)&msg_size, sizeof(int), 0);
        send(s, (const char*)transfer_list[i].c_str(), msg_size, 0);
    }

    return true;
}

std::string Client::get_filename_to_upload(SOCKET& s)
{
    std::string path;
    int msg_size;
    std::string sttr = std::to_string(transfer_list.size());
    msg_size = sttr.size();
    if (recv(s, (char*)&msg_size, sizeof(int), 0) < 0)
        std::cout << "not recv" << std::endl;

    char* msg = new char[msg_size + 1];
    msg[msg_size] = '\0';
    recv(s, msg, msg_size, 0);

    path = msg;
    delete[]msg;
    return path;
}

int Client::get_transfer_list_size() const
{
    return transfer_list.size();
}
