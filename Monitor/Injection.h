#pragma once
#include <winsock2.h> 
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable  : 4996);
#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <comdef.h>
#include <vector>
#include <string>
#include <tchar.h>
#include <ShlObj.h>
#define BUFSIZE 512

enum parameters {FUNCTION = 1, 
				HIDE = 2};
void arguments_parse(int num_argument, char** arguments, DWORD* pid, std::string* func, int* param);

