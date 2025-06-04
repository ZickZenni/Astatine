#include "assault.h"

#include "hooks/hooks_manager.h"
#include "logger.h"
#include "renderer.h"
#include "utils/win.h"

#include <MinHook.h>

#include <chrono>
#include <thread>

AssaultCheat* AssaultCheat::s_instance = nullptr;

AssaultCheat::AssaultCheat(const HMODULE module)
{
    m_hwnd = nullptr;
    m_module = module;
    m_thread = GetCurrentThread();
    s_instance = this;
}

AssaultCheat::~AssaultCheat()
{
    Logger::get().shutdown();
    s_instance = nullptr;
}

bool AssaultCheat::run()
{
    if (m_running) {
        return false;
    }

    if (!initialize()) {
        return false;
    }

    LOG_INFO("Initialization complete.");

    m_running = true;

    // ReSharper disable once CppDFAConstantConditions
    // ReSharper disable once CppDFAEndlessLoop
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    LOG_INFO("Shutting down..");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return shutdown();
}

void AssaultCheat::request_shutdown()
{
    m_running = false;
}

HWND AssaultCheat::get_hwnd() const
{
    return m_hwnd;
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
        LOG_CRIT("Failed to initialize MinHook");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    LOG_INFO("MinHook initialized.");

    if ((m_hwnd = find_window()) == nullptr) {
        LOG_CRIT("Failed to find game window");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    LOG_DEBUG("Found HWND: {}", hwnd_to_hex(m_hwnd));

    HooksManager::get().initialize();
    HooksManager::get().enable();
    return true;
}

bool AssaultCheat::shutdown()
{
    HooksManager::get().disable();
    HooksManager::get().shutdown();

    if (MH_Uninitialize() != MH_OK) {
        LOG_CRIT("Failed to uninitialize MinHook");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    LOG_INFO("MinHook shutdown.");

    Logger::get().shutdown();
    return true;
}

bool AssaultCheat::should_shutdown() const
{
    return !m_running;
}

AssaultCheat* AssaultCheat::get()
{
    return s_instance;
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