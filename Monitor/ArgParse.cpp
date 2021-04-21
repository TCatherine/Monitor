#include "Injection.h"

int find_num_text(std::vector <std::string>& list_arguments, const std::string& substring) {
    for (int i = 0; i < list_arguments.size() - 1; i++) {
        if (list_arguments[i] == substring)
            return i;
    }
    return -1;
}

bool is_keyword(std::string& str) {
    if (str == "-pid")
        return TRUE;
    else if (str == "-name")
        return TRUE;
    else if (str == "-func")
        return TRUE;
    else if (str == "-hide")
        return TRUE;
    return FALSE;
}

DWORD get_pid_by_name(const std::string& process_name) {
    HANDLE h_Snap;
    PROCESSENTRY32 pe32;
    DWORD PID = NULL;
    h_Snap = CreateToolhelp32Snapshot(0x00000002, 0);
    if (h_Snap == INVALID_HANDLE_VALUE) {
        std::cout << "Invalid Handle Value! Failed to get PID " << std::endl;
        return FALSE;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32); 

    if (!Process32FirstW(h_Snap, &pe32)) {
        CloseHandle(h_Snap);     
        std::cout << "Process32FirstW is failded! Failed to get PID " << std::endl;
        return(NULL);
    }

    std::wstring w_process_name(process_name.begin(), process_name.end());
    do {
        if (w_process_name == pe32.szExeFile) {
            PID = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(h_Snap, &pe32));

    CloseHandle(h_Snap);
    return PID;
}

DWORD pid_parse(std::vector<std::string>& list_arguments) {
    DWORD pid=NULL;
    int num_arg = 0;
    if ((num_arg = find_num_text(list_arguments, "-pid")) != -1) {
        if (!is_keyword(list_arguments[num_arg + 1])) {
            pid = std::atoi(list_arguments[num_arg + 1].c_str());
            if (pid == NULL) {
                std::cout << "PID error!" << std::endl;
                exit(0);
            }
        }
        else {
            std::cout << "Arguments error!" << std::endl;
            exit(0);
        }
    }

    if ((num_arg = find_num_text(list_arguments, "-name")) != -1) {
        if (!is_keyword(list_arguments[num_arg + 1]) && pid == NULL) {
            pid = get_pid_by_name(list_arguments[num_arg + 1]);
            if (pid == NULL) {
                std::cout << "Process wasn't found!" << std::endl;
                exit(0);
            }
        }
        else {
            std::cout << "Arguments error!" << std::endl;
            exit(0);
        }
    }

    if (pid == NULL) {
        printf("-pid or -name not found");
        exit(0);
    }
    return pid;
}

void arguments_parse(int num_argument, char** arguments, DWORD* pid, std::string* func, int* param) {
	if (num_argument < 5) {
		std::cout << "Arguments error!" << std::endl;
		exit(0);
	}

    std::vector <std::string> list_arguments;

	for (int i = 1; i < num_argument; i++)
		list_arguments.push_back(arguments[i]);

    *pid = pid_parse(list_arguments);
   
    int num_arg = 0;
    if ((num_arg = find_num_text(list_arguments,"-func")) != -1) {
        if (!is_keyword(list_arguments[num_arg + 1])) {
            *func = list_arguments[num_arg + 1];
            *param = FUNCTION;
        }
        else {
            std::cout << "Arguments error!" << std::endl;
            exit(0);
        }
    }
    else if ((num_arg = find_num_text(list_arguments, "-hide")) != -1) {
        if (!is_keyword(list_arguments[num_arg + 1])) {
            *func = list_arguments[num_arg + 1];
            *param = HIDE;
        }
        else {
            std::cout << "Arguments error!" << std::endl;
            exit(0);
        }
    }
    else {
        printf("-func or -hide not found");
        exit(0);
    }
    return;
}