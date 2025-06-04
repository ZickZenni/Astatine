#include "win.h"

#include <sstream>

std::string hwnd_to_hex(const HWND hwnd)
{
    std::stringstream ss;
    ss << "0x" << hwnd;
    return ss.str();
}

std::string hmodule_to_hex(const HMODULE module)
{
    std::stringstream ss;
    ss << "0x" << module;
    return ss.str();
}