#pragma once

#include <windows.h>

/**
 * Represents an internal cheat for the AssaultCube game. This class manages
 * initialization, execution, and cleanup of the cheat's functionalities.
 */
class AssaultCheat {
public:
    explicit AssaultCheat(HMODULE module);

    ~AssaultCheat();

    /**
     * Executes the main routine of the AssaultCheat instance by initializing the necessary resources,
     * enabling the cheat, and performing shutdown operations upon completion.
     *
     * @return A boolean indicating the success of the operation. Returns true if the cheat
     *         runs and shuts down successfully, or false if initialization fails.
     */
    bool run();

    /**
     * Retrieves the module handle associated with this instance of AssaultCheat.
     *
     * @return The handle to the module (HMODULE) that this instance is associated with.
     */
    [[nodiscard]] HMODULE get_module() const;

    /**
     * Retrieves the handle of the thread associated with this instance of AssaultCheat.
     *
     * @return A handle to the thread (HANDLE) that this instance is using.
     */
    [[nodiscard]] HANDLE get_thread() const;

private:
    HWND m_window;
    HMODULE m_module;
    HANDLE m_thread;

private:
    /**
     * Initializes the AssaultCheat instance by setting up required components, such as
     * logging and the hooking library, and locating the game's main window.
     *
     * @return A boolean indicating whether the initialization succeeded. Returns true
     *         if all components were set up and the game's main window was found, or
     *         false if any initialization step failed.
     */
    bool initialize();

    /**
     * Performs the shutdown operations for the AssaultCheat instance by releasing resources,
     * uninitializing the hooking library, and ensuring a proper cleanup.
     *
     * @return A boolean indicating the success of the shutdown operation. Returns true
     *         if all resources are released and uninitialization is successful, or false
     *         if the uninitialization process fails.
     */
    bool shutdown();

private:
    /**
     * Traverses through all top-level windows in the system to locate the
     * game's main window, identified by its class name and title.
     *
     * @return A handle to the game's window (HWND) if found, or nullptr if no matching
     *         window was located.
     */
    static HWND find_window();
};