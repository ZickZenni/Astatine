#pragma once

#include "detour_hook.h"

#include <windows.h>

/**
 * Manages all hooks related to window procedures or other functionalities.
 * Acts as a central controller for enabling, disabling, and managing hooks.
 */
class HooksManager {
public:
    class Definitions {
    public:
        Definitions() = delete;

        static LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static long long* wglSwapBuffers(HDC hdc);
    };

public:
    /**
     * Initializes hooks and detours used in Astatine.
     */
    void initialize();

    /**
     * Safely releases and disables all active hooks.
     */
    void shutdown();

    /**
     * Enables all hooks.
     */
    void enable();

    /**
     * Disables all hooks.
     */
    void disable();

public:
    /**
     * Provides access to the singleton instance of the HooksManager class.
     * Ensures that only one instance of the HooksManager exists throughout the program lifecycle.
     *
     * @return A reference to the singleton instance of the HooksManager.
     */
    static HooksManager& get();

private:
    DetourHook* m_swap_buffers_hook = nullptr;
    WNDPROC m_og_wndproc = nullptr;

    friend Definitions;

private:
    /**
     * Retrieves the address of a specified procedure from a given module.
     * This function enables the dynamic lookup of functions within the specified
     * module using their names, allowing the retrieval of function pointers at runtime.
     * If the module cannot be found, the function will return a null pointer.
     *
     * @param module_name The name of the module to search for the procedure.
     * @param proc_name The name of the procedure to retrieve the address for.
     * @return A pointer to the procedure, or nullptr if the module or procedure is not found.
     */
    static void* get_proc_address(const char* module_name, const char* proc_name);
};