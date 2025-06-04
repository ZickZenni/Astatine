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

        if (GetStdHandle(STD_OUTPUT_HANDLE) != nullptr) {
            SetConsoleTitleA("AssaultCube Internal");
            SetConsoleOutputCP(CP_UTF8);
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

void Logger::log(LogLevel level, const std::string& msg)
{
    std::string level_str = "UNKNOWN";
    switch (level) {
    case LogLevel::INFO:
        level_str = "INFO";
        break;
    case LogLevel::WARN:
        level_str = "WARN";
        break;
    case LogLevel::ERR:
        level_str = "ERROR";
        break;
    case LogLevel::CRIT:
        level_str = "CRIT";
        break;
    case LogLevel::DEBUG:
        level_str = "DEBUG";
        break;
    default:
        break;
    }

    const std::string formatted_message = std::format("[{}] {}", level_str, msg);
    std::cout << formatted_message << std::endl;
}

Logger& Logger::get()
{
    static Logger logger;
    return logger;
}