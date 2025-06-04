#include "hooks_manager.h"

#include "../assault.h"
#include "../logger.h"
#include "../utils/win.h"

void HooksManager::initialize()
{
    m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(AssaultCheat::get()->get_hwnd(), GWLP_WNDPROC, reinterpret_cast<LONG>(&Hooks::wndproc)));
    m_swap_buffers_hook = new DetourHook(get_proc_address("opengl32.dll", "wglSwapBuffers"), &Hooks::wglSwapBuffers);
    LOG_INFO("HooksManager initialized.");
}

void HooksManager::shutdown()
{
    SetWindowLongPtrW(AssaultCheat::get()->get_hwnd(), GWLP_WNDPROC, reinterpret_cast<LONG>(m_og_wndproc));
    delete m_swap_buffers_hook;
    LOG_INFO("HooksManager shutdown.");
}

void HooksManager::enable()
{
    m_swap_buffers_hook->enable();
}

void HooksManager::disable()
{
    m_swap_buffers_hook->disable();
}

HooksManager& HooksManager::get()
{
    static HooksManager manager;
    return manager;
}

void* HooksManager::get_proc_address(const char* module_name, const char* proc_name)
{
    const auto module = GetModuleHandleA(module_name);
    LOG_DEBUG("HooksManager::get_proc_address(): Module: {} Proc: {} Address: {}", module_name, proc_name, module != nullptr ? hmodule_to_hex(module) : "0x00000000");
    if (module == nullptr) {
        return nullptr;
    }
    return GetProcAddress(module, proc_name);
}