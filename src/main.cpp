#include "MinHook.h"
#include "assault.h"

#include <iostream>
#include <thread>
#include <windows.h>

BOOL APIENTRY DllMain(const HMODULE module, const DWORD reason, PVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        CreateThread(nullptr, 0, [](const PVOID info) -> DWORD {
            auto* assault = new AssaultCheat(static_cast<HMODULE>(info));

            const auto state = assault->run();
            const auto module = assault->get_module();
            const auto thread = assault->get_thread();
            delete assault;

            CloseHandle(thread);
            FreeLibraryAndExitThread(module, state); }, module, 0, nullptr);
    }
    return TRUE;
}
