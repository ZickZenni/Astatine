#include "logger.h"

#include <cstdio>
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

void Logger::info(const std::string& msg)
{
    std::cout << msg << std::endl;
}

Logger& Logger::get()
{
    static Logger logger;
    return logger;
}