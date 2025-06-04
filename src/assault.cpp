#include "assault.h"

#include "logger.h"
#include "utils/win.h"

#include <MinHook.h>

#include <chrono>
#include <thread>

AssaultCheat::AssaultCheat(const HMODULE module)
{
    m_window = nullptr;
    m_module = module;
    m_thread = GetCurrentThread();
}

AssaultCheat::~AssaultCheat()
{
    Logger::get().shutdown();
}

bool AssaultCheat::run()
{
    if (!initialize()) {
        return false;
    }

    INFO("AssaultCube Internal Cheat enabled.");

    return shutdown();
}

HMODULE AssaultCheat::get_module() const
{
    return m_module;
}

HANDLE AssaultCheat::get_thread() const
{
    return m_thread;
}

bool AssaultCheat::initialize()
{
    Logger::get().initialize(true);

    if (MH_Initialize() != MH_OK) {
        INFO("Failed to initialize MinHook");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    if ((m_window = find_window()) == nullptr) {
        INFO("Failed to find game window");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    INFO("Game Window HWND: {}", hwnd_to_hex(m_window));
    return true;
}

bool AssaultCheat::shutdown()
{
    Logger::get().shutdown();

    if (MH_Uninitialize() != MH_OK) {
        INFO("Failed to uninitialize MinHook");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    return true;
}

HWND AssaultCheat::find_window()
{
    for (HWND hwnd = GetTopWindow(nullptr); hwnd != nullptr; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
        if (!IsWindowVisible(hwnd)) {
            continue;
        }

        const int title_length = GetWindowTextLength(hwnd);
        if (title_length == 0) {
            continue;
        }

        const auto title = new char[title_length + 1];
        const auto class_name = new char[512];

        GetWindowText(hwnd, title, title_length + 1);
        GetClassName(hwnd, class_name, 512);

        /*
         * We found our game window.
         */
        if (strcmp(class_name, "SDL_app") == 0 && strcmp(title, "AssaultCube") == 0) {
            delete title;
            delete class_name;
            return hwnd;
        }

        delete title;
        delete class_name;
    }
    return nullptr;
}