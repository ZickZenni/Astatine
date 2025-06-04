#include "MinHook.h"
#include "assault.h"

#include <iostream>
#include <thread>
#include <windows.h>

BOOL APIENTRY DllMain(const HMODULE module, const DWORD reason, PVOID)
{
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        const auto main_thread = CreateThread(nullptr, 0, [](const PVOID info) -> DWORD {
            auto* assault = new AssaultCheat(static_cast<HMODULE>(info));

            const auto state = assault->run();
            const auto module = assault->get_module();
            const auto thread = assault->get_thread();
            delete assault;

            CloseHandle(thread);
            FreeLibraryAndExitThread(module, state); }, module, 0, nullptr);

        if (main_thread == nullptr || main_thread == INVALID_HANDLE_VALUE) {
            const DWORD error = GetLastError();
            std::cerr << "Error creating thread, ERROR CODE: " << error << std::endl;
            return FALSE;
        }
    }
    return TRUE;
}
