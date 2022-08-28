#include <windows.h>

// using the user32.lib
#pragma comment (lib, "user32.lib")

BOOL APIENTRY DllMain(HMODULE hModule,  DWORD  nReason, LPVOID lpReserved) {
  switch (nReason) {
  case DLL_PROCESS_ATTACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  }
  return TRUE;
}

// __declspec(export) --> exporting this function
extern "C" __declspec(dllexport) int Meow() {
  MessageBox(
    NULL,
    "Meow from evil.dll!",
    "=^..^=",
    MB_OK
  );
  return 0;
}