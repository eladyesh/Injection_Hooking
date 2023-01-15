#include <Windows.h>
#include <iostream>


HHOOK hHook = NULL;
BOOL(WINAPI* TrueWriteProcessMemory)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten) = WriteProcessMemory;

LRESULT CALLBACK MyWriteProcessMemoryProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
          // LOG
    }

    // Call the original WriteProcessMemory function
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hHook = SetWindowsHookEx(WH_CBT, MyWriteProcessMemoryProc, hModule, 0);
        break;
    case DLL_PROCESS_DETACH:
        UnhookWindowsHookEx(hHook);
        break;
    }
    return TRUE;
}
