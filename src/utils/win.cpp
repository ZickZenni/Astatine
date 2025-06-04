#include "win.h"

#include <sstream>

std::string hwnd_to_hex(const HWND hwnd)
{
    std::stringstream ss;
    ss << "0x" << hwnd;
    return ss.str();
}