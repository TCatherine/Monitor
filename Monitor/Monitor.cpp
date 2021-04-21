#define WIN32_LEAN_AND_MEAN 
#include "Injection.h"
static const int max_len = 512;
static const char name_lib[] = "hook.dll";

char* get_full_path() {
    char path_lib[BUFSIZE];
    GetModuleFileNameA(NULL, path_lib, BUFSIZE);
    size_t len = sizeof(path_lib);
    while (path_lib[--len] != '\\')
        path_lib[len] = 0;
    strncat_s(path_lib, name_lib, BUFSIZE);
    return path_lib;
}

int load_lib(DWORD PID) {
    std::string lib = get_full_path();

    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (h_process == NULL) {
        std::cout << "OpenProcess error code: " << GetLastError() << std::endl;
        return 0;
    }

    HMODULE module = GetModuleHandleA("kernel32.dll");
    if (module == NULL) {
        std::cout << "OpenProcess error code: " << GetLastError() << std::endl;
        CloseHandle(h_process);
        return 0;
    }

    LPVOID p_LoadLibrary = GetProcAddress(module, "LoadLibraryA");
    if (p_LoadLibrary == NULL) {
        std::cout << "GetProcAddress error code: " << GetLastError() << std::endl;
        CloseHandle(h_process);
        return 0;
    }

    LPVOID lib_name = (LPVOID)VirtualAllocEx(h_process, NULL, lib.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (lib_name == NULL) {
        std::cout << "VirtualAllocEx error code: " << GetLastError() << std::endl;
        CloseHandle(h_process);
        return 0;
    }

    int ret = WriteProcessMemory(h_process, lib_name, lib.c_str(), lib.length(), NULL);
    if (ret == NULL) {
        std::cout << "WriteProcessMemory error code: " << GetLastError() << std::endl;
        CloseHandle(h_process);
        return 0;
    }

    HANDLE h_thread = CreateRemoteThread(h_process, NULL, 0, (LPTHREAD_START_ROUTINE)p_LoadLibrary, lib_name, NULL, NULL);
    if (h_thread == NULL) {
        std::cout << "CreateRemoteThread error code: " << GetLastError() << std::endl;
        CloseHandle(h_process);
        return 0;
    }

    CloseHandle(h_process);
    return 1;
}

void handle_error(const std::string& msg, SOCKET socket) {
    std::cout << msg << " (error: " << WSAGetLastError() << ")" << std::endl;
    closesocket(socket);
    WSACleanup();
    exit(0);
}

void output_statistic(SOCKET client_socket) {
    char recvbuf[max_len];

    while (1)
    {
        SYSTEMTIME st;
        int err = recv(client_socket, reinterpret_cast<char*>(&st), sizeof(SYSTEMTIME)+1, 0);
        sprintf(recvbuf, "%02d:%02d:%02d %02d-%02d-", st.wHour, st.wMinute, st.wSecond, st.wMonth, st.wDay);
        if (err > 0) {
            recvbuf[err] = 0;
        }
        else if (err == 0) {
            std::cout << "Connection closing..." << std::endl;
            break;
        }
        else {
            handle_error("recv failed", client_socket);
        }
        std::cout << "Time: " << recvbuf << st.wYear << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (!IsUserAnAdmin()) {
        std::cout << "Administrator privileges required" << std::endl;
        return 0;
    }

    std::string func_or_hide;
    DWORD PID;
    int parameter;
    arguments_parse(argc, argv, &PID, &func_or_hide, &parameter);    


    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket, client_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in server_addr;  
    server_addr.sin_family = AF_INET;
    InetPton(AF_INET, _T("169.254.198.104"), &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9000);

    int err;

    // Create a SOCKET for connecting to server
    // Setup the TCP listening socket
    err = bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (err == SOCKET_ERROR) {
        handle_error("bind failed", server_socket);
    }

    err = listen(server_socket, 50);
    if (err == SOCKET_ERROR) {
        handle_error("listen failed", server_socket);
    }

    load_lib(PID);

    // Accept a client socket
    client_socket = accept(server_socket, NULL, NULL);
   
    char recvbuf[max_len];
    err = recv(client_socket, recvbuf, max_len, 0);

    if (err <= 0)
        handle_error("recv failed", client_socket);

    recvbuf[err] = 0;
    std::cout << "Received query: " << recvbuf << std::endl;
    std::string send_string = std::to_string(parameter) + func_or_hide;
    send(client_socket,send_string.c_str(), send_string.length()+1, 0);
    std::cout << "Sent answer: " << send_string << std::endl;

    if (parameter == FUNCTION)
        output_statistic(client_socket);

    // shutdown the connection since we're done
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 1;
}