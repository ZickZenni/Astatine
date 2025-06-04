#include "pointer.h"

#include <sstream>

std::string ptrthex(void* ptr)
{
    std::stringstream ss;
    ss << "0x" << reinterpret_cast<unsigned long long>(ptr);
    return ss.str();
}