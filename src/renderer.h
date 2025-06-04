#pragma once

#include <windows.h>

/**
 * The Renderer class is responsible for managing the rendering system,
 * including setup, frame management, and shutdown of ImGui within an
 * OpenGL/Win32 environment. It also provides a singleton interface for
 * global access.
 */
class Renderer {
public:
    bool mMenuOpen = false;

public:
    /**
     * Initializes the rendering system and the associated ImGui context.
     * This method sets up ImGui for use with OpenGL and Win32 and sets the appropriate configurations.
     *
     * @param hwnd A handle to the window (HWND) that will be used for rendering. This is required
     *             to initialize ImGui for Win32.
     */
    void initialize(HWND hwnd);

    /**
     * Shuts down the rendering system and cleans up the ImGui context.
     * This method deinitializes the ImGui implementation for OpenGL and Win32,
     * destroys the ImGui context, and resets the renderer's state.
     */
    void shutdown();

    /**
     * Starts a new frame for the rendering system.
     * This method prepares the ImGui context by creating new frames for the OpenGL and Win32 platforms
     * and initializes ImGui for the current frame.
     */
    void begin_frame();

    /**
     * Completes the current rendering frame and submits draw data to the GPU.
     * This method triggers the rendering of ImGui's prepared draw data using the OpenGL backend.
     */
    void end_frame();

    /**
     * Checks if the rendering system has been initialized.
     * This method determines whether the renderer has gone through
     * the initialization process and is ready for use.
     *
     * @return True if the renderer is initialized; otherwise, false.
     */
    [[nodiscard]] bool is_initialized() const;

public:
    /**
     * Provides a reference to the singleton instance of the Renderer.
     * This method ensures there is only one instance of the Renderer
     * and provides global access to it.
     *
     * @return A reference to the singleton Renderer instance.
     */
    static Renderer& get();

private:
    bool m_initialized = false;
};