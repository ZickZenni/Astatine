#include "astatine.h"

#include "hooks/hooks_manager.h"
#include "logger.h"
#include "utils/pointer.h"

#include <MinHook.h>

#include <chrono>
#include <thread>

Astatine* Astatine::s_instance = nullptr;

Astatine::Astatine(const HMODULE module)
{
    m_hwnd = nullptr;
    m_module = module;
    m_thread = GetCurrentThread();
    m_running = false;
    s_instance = this;
}

Astatine::~Astatine()
{
    Logger::get().shutdown();
    s_instance = nullptr;
}

bool Astatine::run()
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

void Astatine::request_shutdown()
{
    m_running = false;
}

HWND Astatine::get_hwnd() const
{
    return m_hwnd;
}

HMODULE Astatine::get_module() const
{
    return m_module;
}

HANDLE Astatine::get_thread() const
{
    return m_thread;
}

bool Astatine::initialize()
{
    /*
     * Initialize the logger (+ with a console allocated)
     */
    Logger::get().initialize(true);

    LOG_RAW(R"(
    ░▒▓██████▓▒░ ░▒▓███████▓▒░▒▓████████▓▒░▒▓██████▓▒░▒▓████████▓▒░▒▓█▓▒░▒▓███████▓▒░░▒▓████████▓▒░
   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░         ░▒▓█▓▒░  ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░         ░▒▓█▓▒░  ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
   ░▒▓████████▓▒░░▒▓██████▓▒░   ░▒▓█▓▒░  ░▒▓████████▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██████▓▒░
   ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░  ░▒▓█▓▒░  ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
   ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░  ░▒▓█▓▒░  ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
   ░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░   ░▒▓█▓▒░  ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░
)");

    /*
     * Initialize MinHook.
     */
    if (MH_Initialize() != MH_OK) {
        LOG_CRIT("Failed to initialize MinHook");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    LOG_INFO("MinHook initialized.");

    /*
     * Find the game's hwnd.
     */
    if ((m_hwnd = find_window()) == nullptr) {
        LOG_CRIT("Failed to find game window");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    LOG_DEBUG("Found HWND: {}", ptrthex(m_hwnd));

    /*
     * Initialize all hooks and enable them.
     */
    HooksManager::get().initialize();
    HooksManager::get().enable();
    return true;
}

// ReSharper disable once CppDFAUnreachableFunctionCall
bool Astatine::shutdown()
{
    /*
     * Disable and shut down all hooks.
     */
    HooksManager::get().disable();
    HooksManager::get().shutdown();

    /*
     * Uninitialize MinHook.
     */
    if (MH_Uninitialize() != MH_OK) {
        LOG_CRIT("Failed to uninitialize MinHook");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return false;
    }

    LOG_INFO("MinHook shutdown.");

    /*
     * Shutdown the logger.
     */
    Logger::get().shutdown();
    return true;
}

bool Astatine::should_shutdown() const
{
    return !m_running;
}

Astatine* Astatine::get()
{
    return s_instance;
}

HWND Astatine::find_window()
{
    /*
     * Somehow using FindWindow with "SDL_app" and "AssaultCube" does not work.
     * This works and could be obviously better, but it's only run one time at the start,
     * so it's okay.
     */
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