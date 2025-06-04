#include "logger.h"

#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <windows.h>

void Logger::initialize(const bool alloc_console)
{
    if (alloc_console) {
        if (AttachConsole(GetCurrentProcessId()) == false)
            AllocConsole();

        if (HANDLE handle; (handle = GetStdHandle(STD_OUTPUT_HANDLE)) != nullptr) {
            SetConsoleTitleA("Astatine");
            SetConsoleOutputCP(CP_UTF8);

            /*
             * Enable ASCII escape colors.
             */
            DWORD consoleMode;
            GetConsoleMode(handle, &consoleMode);
            consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
            SetConsoleMode(handle, consoleMode);

            /*
             * Open a stream through we can use the standard std::cout to print messages to the newly allocated console.
             */
            freopen_s(&m_console_stream, "CONOUT$", "w", stdout);
            m_console_exists = true;
        }
    }
}
void Logger::shutdown()
{
    if (m_console_exists) {
        if (m_console_stream != nullptr) {
            fclose(m_console_stream);
        }
        FreeConsole();
        m_console_exists = false;
        m_console_stream = nullptr;
    }
}

void Logger::log(const LogLevel level, const std::string& msg)
{
    std::string level_str = "UNKNOWN";

    /*
     * ANSI escape color codes.
     * https://stackoverflow.com/a/54062826/22032371
     */
    std::string level_col = "37";

    switch (level) {
    case LogLevel::RAW:
        std::cout << msg << std::endl;
        return;
    case LogLevel::INFO:
        level_str = "INFO ";
        level_col = "32";
        break;
    case LogLevel::WARN:
        level_str = "WARN ";
        level_col = "33";
        break;
    case LogLevel::ERR:
        level_str = "ERROR";
        level_col = "91";
        break;
    case LogLevel::CRIT:
        level_str = "CRIT ";
        level_col = "31";
        break;
    case LogLevel::DEBUG:
        level_str = "DEBUG";
        level_col = "36";
        break;
    default:
        break;
    }

    /*
     * Using a variable for coloring is stupid, but it will work. So don't fucking fix it.
     */
    const std::string formatted_message = std::format("\033[{}m{}\033[0m: {}", level_col, level_str, msg);
    std::cout << formatted_message << std::endl;
}

Logger& Logger::get()
{
    static Logger logger;
    return logger;
}