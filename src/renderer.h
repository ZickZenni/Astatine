#pragma once

#include <windows.h>

class Renderer {
public:
    bool mMenuOpen = false;

public:
    void initialize(HWND hwnd);

    void shutdown();

    void begin_frame();

    void end_frame();

    [[nodiscard]] bool is_initialized() const;

public:
    static Renderer& get();

private:
    bool m_initialized = false;
};