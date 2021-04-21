#include "pch.h"
#include "Hook.h"

char dt[MAXLEN];
char timer[MAXLEN] = { 0 };
LPVOID true_function = NULL;

void get_statistics()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(dt, "%02d:%02d:%02d %02d-%02d-%d\n", st.wHour, st.wMinute, st.wSecond, st.wMonth, st.wDay, st.wYear);
    if (strcmp(dt, timer)){
        OutputDebugStringA("[MY DLL] THERE ARE IN  HOOK!\n");
        char msg[MAXLEN];
        strncpy(timer, dt, strlen(dt)+1);
        sprintf(msg, "Time: %s", timer);
        send(connect_socket, &st, sizeof(st), 0);
        OutputDebugStringA("[MY DLL] ");
        OutputDebugStringA(msg);
    }
}

#ifdef _WIN64
void Hook_x64();
#else
void __declspec (naked) hook_x86() {
    __asm {
        pushfd
        pushad
        call get_statistics
        popad
        popfd

        push true_function
        ret
    }
}
#endif


void attach_statistic() {
    true_function = DetourFindFunction("kernel32.dll", name);
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
#ifdef _WIN64
    DetourAttach(&(PVOID)true_function, Hook_x64);
#else
    DetourAttach(&(PVOID)true_function, hook_x86);
#endif
    LONG err = DetourTransactionCommit();
    if (err != NO_ERROR) {
        char sendMsg[MAXLEN];
        sprintf_s(sendMsg, "ERROR: DetourTransactionCommit() - %d\n", err);
        send(connect_socket, sendMsg, strlen(sendMsg) + 1, 0);
        return 1;
    }
}