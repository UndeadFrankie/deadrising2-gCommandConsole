// ConsoleInjector.cpp
#include "pch.h"
#include <windows.h>
#include <iostream>

// Function pointer type
typedef void(__cdecl* tReadLine)(const char** buffer);
const uintptr_t FUNC_ADDR = 0x0A6B500; // Replace with actual address if needed

DWORD WINAPI MainThread(LPVOID lpReserved) {
    // Set up console I/O
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);

    std::cout << "[+] Debug console attached. Type a command and press Enter.\n";
    std::cout << "[*] Press F2 to unload.\n";

    tReadLine ReadLine = (tReadLine)FUNC_ADDR;
    char inputBuffer[512];

    while (true) {
        std::cout << "> ";
        std::cin.getline(inputBuffer, sizeof(inputBuffer));

        // Check exit key (F2)
        if (GetAsyncKeyState(VK_F2) & 1)
            break;

        // Skip empty lines
        if (inputBuffer[0] == '\0')
            continue;

        const char* ptr = inputBuffer;
        ReadLine(&ptr);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
    }
    return TRUE;
}