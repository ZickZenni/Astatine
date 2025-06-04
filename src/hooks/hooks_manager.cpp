#include "hooks_manager.h"

#include "../astatine.h"
#include "../logger.h"
#include "../utils/pointer.h"

void HooksManager::initialize()
{
    m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(Astatine::get()->get_hwnd(), GWLP_WNDPROC, reinterpret_cast<LONG>(&Hooks::wndproc)));
    LOG_DEBUG("HooksManager::initialize(): WndProc Original: {} Detour: {}", ptrthex(m_og_wndproc),  ptrthex(&Hooks::wndproc));

    {
        const auto target = get_proc_address("opengl32.dll", "wglSwapBuffers");
        const auto detour = &Hooks::wglSwapBuffers;

        m_swap_buffers_hook = new DetourHook(target, detour);
        LOG_DEBUG("HooksManager::initialize(): SwapBuffers Original: {} Detour: {}", ptrthex(target), ptrthex(detour));
    }

    LOG_INFO("HooksManager initialized.");
}

void HooksManager::shutdown()
{
    SetWindowLongPtrW(Astatine::get()->get_hwnd(), GWLP_WNDPROC, reinterpret_cast<LONG>(m_og_wndproc));
    delete m_swap_buffers_hook;
    LOG_INFO("HooksManager shutdown.");
}

void HooksManager::enable()
{
    m_swap_buffers_hook->enable();
    LOG_DEBUG("HooksManager enabled");
}

void HooksManager::disable()
{
    m_swap_buffers_hook->disable();
    LOG_DEBUG("HooksManager disabled");
}

HooksManager& HooksManager::get()
{
    static HooksManager manager;
    return manager;
}

void* HooksManager::get_proc_address(const char* module_name, const char* proc_name)
{
    const auto module = GetModuleHandleA(module_name);
    LOG_DEBUG("HooksManager::get_proc_address(): Module: {} Proc: {} Address: {}", module_name, proc_name, module != nullptr ?  ptrthex(module) : "0x00000000");
    if (module == nullptr) {
        return nullptr;
    }
    return GetProcAddress(module, proc_name);
}