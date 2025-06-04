#include "MinHook.h"
#include "astatine.h"

#include <iostream>
#include <thread>
#include <windows.h>

BOOL APIENTRY DllMain(const HMODULE module, const DWORD reason, PVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        CreateThread(nullptr, 0, [](const PVOID info) -> DWORD {
            auto* astatine = new Astatine(static_cast<HMODULE>(info));

            const auto state = astatine->run();
            const auto module = astatine->get_module();
            const auto thread = astatine->get_thread();
            delete astatine;

            CloseHandle(thread);
            FreeLibraryAndExitThread(module, state); }, module, 0, nullptr);
    }
    return TRUE;
}
