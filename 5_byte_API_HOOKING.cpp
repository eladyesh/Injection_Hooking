#include <Windows.h>
#include <iostream>
#include "pch.h"
#include <sstream>
#include <winsock.h>
#include <chrono>
#include <tlhelp32.h>
using std::ostringstream;
using std::ends;

typedef BOOL(WINAPI* TrueWriteProcessMemory)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);
TrueWriteProcessMemory originalWriteProcessMemory = WriteProcessMemory;

unsigned char originalBytes[5];
unsigned char trampoline[5];

HANDLE hFile;

template<typename T>
void LOG(const char* message, T parameter) {

    WriteFile(hFile, message, strlen(message), NULL, nullptr);
    //WriteFile(hFile, "\n", strlen("\n"), NULL, nullptr);
    ostringstream oss;
    ostringstream oss2;
    oss << parameter << ends;
    WriteFile(hFile, oss.str().c_str(), strlen(oss.str().c_str()), NULL, nullptr);
    WriteFile(hFile, "\n", strlen("\n"), NULL, nullptr);

}

BOOL WINAPI MyWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten)
{
    // Perform any desired actions here (e.g. logging)
    LOG("\n----------intercepted call to WriteProcessMemory----------\n\n", "");
    LOG("A pointer to the base address in the specified process to which data is written is ", lpBaseAddress);
    LOG("A pointer to the buffer that contains data to be written in the address space of the specified process is ", (LPCSTR)lpBuffer);
    LOG("The number of bytes to be written to the specified process is ", nSize);
    LOG("\n----------Done intercepting call to WriteProcessMemory----------\n\n", "");

    DWORD oldProtection;
    VirtualProtect(WriteProcessMemory, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
    memcpy(WriteProcessMemory, trampoline, 5);
    VirtualProtect(WriteProcessMemory, 5, oldProtection, &oldProtection);

    BOOL ret = originalWriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);

    VirtualProtect(WriteProcessMemory, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
    memcpy(WriteProcessMemory, originalBytes, 5);
    VirtualProtect(WriteProcessMemory, 5, oldProtection, &oldProtection);

    return ret;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {

        hFile = CreateFile(L"LOG_MEMORY.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

        // Save the original address of the WriteProcessMemory function
        originalWriteProcessMemory = WriteProcessMemory;

        // Create a jump instruction to our custom function
        unsigned char jump[5] = { 0xE9 };
        *reinterpret_cast<unsigned long*>(&jump[1]) = (unsigned long)MyWriteProcessMemory - (unsigned long)WriteProcessMemory - 5;

        DWORD oldProtection;
        VirtualProtect(WriteProcessMemory, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
        memcpy(originalBytes, WriteProcessMemory, 5);
        memcpy(trampoline, &originalBytes, 5);
        *reinterpret_cast<unsigned long*>(&trampoline[1]) = (unsigned long)WriteProcessMemory + 5 - (unsigned long)trampoline - 5;
        memcpy(WriteProcessMemory, jump, 5);
        break;
    }
    case DLL_PROCESS_DETACH:
        // Cleanup any allocated resources
        DWORD oldProtection;
        VirtualProtect(WriteProcessMemory, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
        memcpy(WriteProcessMemory, originalBytes, 5);
        VirtualProtect(WriteProcessMemory, 5, oldProtection, &oldProtection);
        break;
    }
    return TRUE;
}
