#pragma once

#include <format>
#include <string>

class Logger {
public:
    /**
     * Initializes the Logger by optionally attaching or allocating a console.
     * If a console is initialized successfully, the standard output is redirected to it.
     *
     * @param alloc_console Indicates whether to attach or allocate a new console.
     *                       If true, attempts to attach to an existing console or allocate a new one.
     *                       If false, no console is initialized or modified.
     */
    void initialize(bool alloc_console);

    /**
     * Shuts down the Logger by releasing associated console resources.
     * If a console was initialized or attached, it ensures the resources
     * are properly released, including closing the stream and freeing the console.
     */
    void shutdown();

    template<typename... args_t>
    void info(const std::string_view fmt, args_t&&... args)
    {
        const std::string formatted_message = std::vformat(fmt, std::make_format_args(args...));
        info(formatted_message);
    }

    void info(const std::string& msg);

public:
    static Logger& get();

private:
    FILE* m_console_stream = nullptr;
    bool m_console_exists = false;
};

#define INFO(message, ...) Logger::get().info(message, __VA_ARGS__)