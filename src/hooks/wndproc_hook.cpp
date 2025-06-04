#include "../astatine.h"
#include "../logger.h"
#include "../renderer.h"
#include "hooks_manager.h"

LRESULT HooksManager::Definitions::wndproc(const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam)
{
    switch (msg) {
    case WM_KEYDOWN: {
        if (wparam == VK_F4) {
            Renderer::get().mMenuOpen = !Renderer::get().mMenuOpen;
        }
        if (wparam == VK_F8) {
            Astatine::get()->request_shutdown();
        }
        break;
    }
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
    case WM_MOUSEMOVE: {
        if (Renderer::get().mMenuOpen) {
            return FALSE;
        }
        break;
    }
    }

    return CallWindowProc(HooksManager::get().m_og_wndproc, hwnd, msg, wparam, lparam);
}