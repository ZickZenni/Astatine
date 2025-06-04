#include "detour_hook.h"

#include "MinHook.h"

#include <stdexcept>

DetourHook::DetourHook(void* target, void* detour)
{
    m_target = target;
    m_detour = detour;
    m_original = nullptr;

    if (MH_CreateHook(target, detour, &m_original) != MH_OK) {
        throw std::runtime_error("Failed to create hook");
    }
}

DetourHook::~DetourHook()
{
    if (m_target) {
        MH_RemoveHook(m_target);
    }
}

void DetourHook::enable() const
{
    if (MH_EnableHook(m_target) != MH_OK) {
        throw std::runtime_error("Failed to enable hook");
    }
}

void DetourHook::disable() const
{
    if (MH_DisableHook(m_target) != MH_OK) {
        throw std::runtime_error("Failed to disable hook");
    }
}