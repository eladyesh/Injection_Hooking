#include <windows.h>
#include <cstdio>
#include "pch.h"

typedef VOID(__cdecl* CatProc)(); // cat
typedef VOID(__cdecl* BirdProc)(); // bird
typedef VOID(__cdecl* MouseProc)(); // mouse

int main(void) {

    // main dll with exported functions
    HMODULE petDll;

    // pets
    CatProc catFunc;
    BirdProc birdFunc;
    MouseProc mouseFunc;

    // free memory
    BOOL freeRes;

    petDll = LoadLibrary(TEXT("C:\\Users\\elady\\Downloads\\vs_tests\\malware\\malware\\pet.dll"));

    if (petDll != NULL) {

        // finding address and activating functions of pets
        catFunc = (CatProc)GetProcAddress(petDll, "Cat");
        birdFunc = (BirdProc)GetProcAddress(petDll, "Bird");
        mouseFunc = (MouseProc)GetProcAddress(petDll, "Mouse");

        if (catFunc != NULL) {
            (catFunc)();
        }
        else { std::cout << "Error: " << GetLastError() << std::endl; }
        if (birdFunc != NULL) {
            (birdFunc)();
        }
        if (mouseFunc != NULL) {
            (mouseFunc)();
        }

        // freeing dll from memory
        freeRes = FreeLibrary(petDll);
        if (freeRes != 0) 
            printf("All done in the hood");
    }

    else 
    {
        DWORD dwError = 0;
        dwError = GetLastError();
        std::cout << "Error: " << dwError << std::endl;
    }

    return 0;
}