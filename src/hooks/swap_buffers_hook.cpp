#include "../logger.h"

#include "../astatine.h"
#include "../renderer.h"
#include "backends/imgui_impl_win32.h"
#include "hooks_manager.h"

long long* HooksManager::Hooks::wglSwapBuffers(const HDC hdc)
{
    /*
     * Shutdown renderer if the cheats should shut down.
     */
    if (Astatine::get()->should_shutdown()) {
        if (Renderer::get().is_initialized()) {
            Renderer::get().shutdown();
            LOG_INFO("Renderer shutdown.");
        }
        return get().m_swap_buffers_hook->get_original<decltype(&wglSwapBuffers)>()(hdc);
    }

    /*
     * Initialize the renderer when not already.
     */
    if (!Renderer::get().is_initialized()) {
        Renderer::get().initialize(Astatine::get()->get_hwnd());
        LOG_INFO("Renderer initialized.");
    }

    /*
     * Actual rendering.
     */
    if (Renderer::get().mMenuOpen) {
        Renderer::get().begin_frame();

        ImGui::Begin("Test");
        ImGui::End();

        Renderer::get().end_frame();
    }

    return get().m_swap_buffers_hook->get_original<decltype(&wglSwapBuffers)>()(hdc);
}