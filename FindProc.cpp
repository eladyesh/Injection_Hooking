#include "pch.h"

#define MSGET(x) {std::cout << x << std::endl; system("pause"); return 0;}

bool compare(const std::wstring& str, char const* c)
{
    std::string c_str(c);
    if (str.size() < c_str.size())
    {
        return false;
    }
    return std::equal(c_str.begin(), c_str.end(), str.begin());
}

std::wstring to_wstring(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

const WCHAR* to_wchar(const char* p) {
    const WCHAR* pwcsName;
    int nChars = MultiByteToWideChar(CP_ACP, 0, p, -1, NULL, 0);
    pwcsName = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, p, -1, (LPWSTR)pwcsName, nChars);
    return pwcsName;
}

DWORD procNameToPID(const char* procName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        MSGET("Error trying to take snapshots of the system processes");

    PROCESSENTRY32 process;
    process.dwSize = sizeof(PROCESSENTRY32);

    Process32First(snapshot, &process);
    do
    {
        if (strstr((char*)process.szExeFile, procName))
            return process.th32ProcessID;

    } while (Process32Next(snapshot, &process));

    return 0;
}

// find process ID by process name
int findMyProc(const wchar_t* procname) {

    HANDLE hSnapshot;
    PROCESSENTRY32 pe;
    int pid = 0;
    BOOL hResult;

    // snapshot of all processes in the system
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

    // initializing size: needed for using Process32First
    pe.dwSize = sizeof(PROCESSENTRY32);

    // info about first process encountered in a system snapshot
    hResult = Process32First(hSnapshot, &pe);

    // retrieve information about the processes
    // and exit if unsuccessful
    while (hResult) {

        // if we find the process: return process ID
        if (procname == (wchar_t*)pe.szExeFile) {
            pid = pe.th32ProcessID;
            std::cout << "The pid is " << pid << std::endl;
            break;
        }
        hResult = Process32Next(hSnapshot, &pe);
    }

    // closes an open handle (CreateToolhelp32Snapshot)
    CloseHandle(hSnapshot);
    return pid;
}

int main(int argc,  char* argv[]) {

    //int pid = 0; // process ID
    //char* p = argv[1];
    //const WCHAR* pwcsName;
    //int nChars = MultiByteToWideChar(CP_ACP, 0, p, -1, NULL, 0);
    //pwcsName = new WCHAR[nChars];
    //MultiByteToWideChar(CP_ACP, 0, p, -1, (LPWSTR)pwcsName, nChars);
    //pid = findMyProc(pwcsName);

    int pid = procNameToPID(argv[1]);
    if (pid) {
        printf("PID = %d\n", pid);
    }
    return 0;
}