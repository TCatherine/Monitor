#include "pch.h"
#include "Hook.h"


HANDLE(__stdcall*pCreateFileA) (LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE(__stdcall*pCreateFileW) (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE(__stdcall*pFindFirstFileW) (LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
HANDLE(__stdcall*pFindFirstFileA)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
BOOL(__stdcall*pFindNextFileW) (HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
BOOL(__stdcall* pFindNextFileA) (HANDLE hFindFile, LPWIN32_FIND_DATAA  lpFindFileData);
HANDLE(__stdcall *pFindFirstFileExA) (LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags);
HANDLE(__stdcall *pFindFirstFileExW) (LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS  fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags);


void get_wstring() {
	int n_chars = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, name, -1, (LPWSTR)w_name, n_chars);
	return;
}

HANDLE __stdcall h_CreateFileA(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	
	if (!strcmp(lpFileName, name)) {
		OutputDebugStringA("[MY DLL] h_CreateFileA: File is matched!\n");
		return INVALID_HANDLE_VALUE;
	}
	OutputDebugStringA("[MY DLL] h_CreateFileA: File isn't matched!\n");
	return pCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE __stdcall h_CreateFileW(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	if (!wcscmp(lpFileName, w_name)) {
		OutputDebugStringA("[MY DLL] h_CreateFileW: File is matched\n");
		return INVALID_HANDLE_VALUE;
	}
	OutputDebugStringA("[MY DLL] h_CreateFileW: File isn't matched\n");
	return pCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HANDLE __stdcall h_FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	if (!strcmp(lpFileName, name)) {
		OutputDebugStringA("[MY DLL] h_FindFirstFileA: File is matched\n");
		return INVALID_HANDLE_VALUE;
	}
	OutputDebugStringA("[MY DLL] h_FindFirstFileA: File isn't matched\n");
	return pFindFirstFileA(lpFileName, lpFindFileData);
}

HANDLE __stdcall h_FindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	if (!wcscmp(lpFileName, w_name)) {
		OutputDebugStringA("[MY DLL] h_FindFirstFileW: File is matched\n");
		return INVALID_HANDLE_VALUE;
	}
	OutputDebugStringA("[MY DLL] h_FindFirstFileW: File isn't matched\n");
	return pFindFirstFileW(lpFileName, lpFindFileData);
}

BOOL __stdcall h_FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
	int ret = pFindNextFileA(hFindFile, lpFindFileData);
	if (!strcmp(lpFindFileData->cFileName, name)) {
		OutputDebugStringA("[MY DLL] h_FindNextFileA: File is matched\n");
		ret = pFindNextFileA(hFindFile, lpFindFileData);
		return ret;
	}
	OutputDebugStringA("[MY DLL] h_FindNextFileA: File isn't matched\n");
	return ret;
}

BOOL __stdcall h_FindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
	int ret = pFindNextFileW(hFindFile, lpFindFileData);
	if (!wcscmp(lpFindFileData->cFileName, w_name)) {
		OutputDebugStringA("[MY DLL] h_FindNextFileW: File is matched\n");
		ret = pFindNextFileW(hFindFile, lpFindFileData);
		return ret;
	}
	OutputDebugStringA("[MY DLL] h_FindNextFileW: File isn't matched\n");
	return ret;
}

HANDLE __stdcall h_FindFirstFileExW(
	LPCWSTR a0,
	FINDEX_INFO_LEVELS a1,
	LPWIN32_FIND_DATAW a2,
	FINDEX_SEARCH_OPS a3,
	LPVOID a4,
	DWORD a5)
{
	HANDLE ret = pFindFirstFileExW(a0, a1, a2, a3, a4, a5);
	if (!wcscmp(a2->cFileName, w_name))
	{
		OutputDebugStringA("[MY DLL] h_FindFirstFileExW: File is matched\n");
		ret = INVALID_HANDLE_VALUE;
		return ret;
	}
	OutputDebugStringA("[MY DLL] h_FindFirstFileExW: File isn't matched\n");
	return ret;
}

HANDLE __stdcall h_FindFirstFileExA(
	LPCSTR a0,
	FINDEX_INFO_LEVELS a1,
	LPWIN32_FIND_DATAA a2,
	FINDEX_SEARCH_OPS a3,
	LPVOID a4,
	DWORD a5)
{
	HANDLE ret = pFindFirstFileExA(a0, a1, a2, a3, a4, a5);
	if (!strcmp(a2->cFileName, name))
	{
		OutputDebugStringA("[MY DLL] h_FindFirstFileExA: File is matched\n");
		ret = INVALID_HANDLE_VALUE;
		return ret;
	}
	OutputDebugStringA("[MY DLL] h_FindFirstFileExA: File isn't matched\n");
	return ret;
}

int attach_hide_file()
{
	OutputDebugStringA("[MY DLL] Attach functions with hide\n");
	get_wstring();

	pCreateFileA = DetourFindFunction("kernel32.dll", "CreateFileA");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID)pCreateFileA,h_CreateFileA);
	LONG err = DetourTransactionCommit();
	if (err != NO_ERROR) {
		OutputDebugStringA("[MY DLL] Error CreateFileA\n");
		return -1;
	}

	pCreateFileW = DetourFindFunction("kernel32.dll", "CreateFileW");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID)pCreateFileW, h_CreateFileW);
	err = DetourTransactionCommit();
	 if (err != NO_ERROR) {
		 OutputDebugStringA("[MY DLL] Error CreateFileW\n");
		 return -1;
	 }

	pFindFirstFileW= DetourFindFunction("kernel32.dll", "FindFirstFileW");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID)pFindFirstFileW, h_FindFirstFileW);
	err = DetourTransactionCommit();
	 if (err != NO_ERROR) {
		 OutputDebugStringA("[MY DLL] Error FindFirstFileW\n");
		 return -1;
	 }


	pFindFirstFileA = DetourFindFunction("kernel32.dll", "FindFirstFileA");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID)pFindFirstFileA, h_FindFirstFileA);
	err = DetourTransactionCommit();
	if (err != NO_ERROR) {
		OutputDebugStringA("[MY DLL] _FindFirstFileA\n");
		return -1;
	}

	pFindNextFileW = DetourFindFunction("kernel32.dll", "FindNextFileW");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID)pFindNextFileW, h_FindNextFileW);
	 err = DetourTransactionCommit();
	 if (err != NO_ERROR) {
		 OutputDebugStringA("[MY DLL] Error FindNextFileW\n");
		 return -1;
	 }

	pFindNextFileA = DetourFindFunction("kernel32.dll", "FindNextFileA");
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID)pFindNextFileA, h_FindNextFileA);
	 err = DetourTransactionCommit();
	 if (err != NO_ERROR) {
		 OutputDebugStringA("[MY DLL] Error FindNextFileA\n");
		 return -1;
	 }

	 pFindFirstFileExW = DetourFindFunction("kernel32.dll", "FindFirstFileExW");
	 DetourTransactionBegin();
	 DetourUpdateThread(GetCurrentThread());
	 DetourAttach(&(PVOID)pFindFirstFileExW, h_FindFirstFileExW);
	 err = DetourTransactionCommit();
	 if (err != NO_ERROR) {
		 OutputDebugStringA("[MY DLL] Error FindFirstFileExW\n");
		 return -1;
	 }

	 pFindFirstFileExA = DetourFindFunction("kernel32.dll", "FindFirstFileExA");
	 DetourTransactionBegin();
	 DetourUpdateThread(GetCurrentThread());
	 DetourAttach(&(PVOID)pFindFirstFileExA, h_FindFirstFileExA);
	 err = DetourTransactionCommit();
	 if (err != NO_ERROR) {
		 OutputDebugStringA("[MY DLL] Error FindFirstFileExA\n");
		 return -1;
	 }
	 OutputDebugStringA("[MY DLL] Success!\n");
    return 0;
}