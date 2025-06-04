#pragma once

#include <format>
#include <string>

/**
 * Logger class provides logging functionality with categorized log levels.
 * It supports formatted logging, console initialization, and controlled shutdown.
 */
class Logger {
    /**
     * LogLevel is an enum class defining severity levels for logging.
     * It categorizes log messages into distinct levels for better filtering and readability.
     *
     * - INFO: General informational messages.
     * - WARN: Indications of potentially harmful situations.
     * - ERR: Error events that might allow the application to continue running.
     * - CRIT: Critical conditions indicating severe errors or the need for immediate attention.
     * - DEBUG: Debug-level messages for detailed troubleshooting.
     */
public:
    enum class LogLevel {
        INFO,
        WARN,
        ERR,
        CRIT,
        DEBUG,
    };

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
        log(LogLevel::INFO, fmt, args...);
    }

    template<typename... args_t>
    void warn(const std::string_view fmt, args_t&&... args)
    {
        log(LogLevel::WARN, fmt, args...);
    }

    template<typename... args_t>
    void error(const std::string_view fmt, args_t&&... args)
    {
        log(LogLevel::ERR, fmt, args...);
    }

    template<typename... args_t>
    void crit(const std::string_view fmt, args_t&&... args)
    {
        log(LogLevel::CRIT, fmt, args...);
    }

    template<typename... args_t>
    void debug(const std::string_view fmt, args_t&&... args)
    {
        log(LogLevel::DEBUG, fmt, args...);
    }

    template<typename... args_t>
    void log(const LogLevel level, const std::string_view fmt, args_t&&... args)
    {
        const std::string formatted_message = std::vformat(fmt, std::make_format_args(args...));
        log(level, formatted_message);
    }

    /**
     * Logs a message with a specified log level, formatting it and printing it to the standard output.
     * The log level indicates the severity or purpose of the message.
     *
     * @param level The severity level of the message to log. This determines the log level prefix
     *              (e.g., INFO, WARN, ERR, CRIT, DEBUG).
     * @param msg   The message content to log. This should be a preformatted or plain text string.
     */
    void log(LogLevel level, const std::string& msg);

public:
    /**
     * Provides access to the singleton instance of the Logger.
     * This ensures a single, globally accessible instance of the Logger
     * is created and used throughout the application.
     *
     * @return A reference to the singleton Logger instance.
     */
    static Logger& get();

private:
    FILE* m_console_stream = nullptr;
    bool m_console_exists = false;
};

#define LOG_INFO(message, ...) Logger::get().info(message, __VA_ARGS__)
#define LOG_WARN(message, ...) Logger::get().warn(message, __VA_ARGS__)
#define LOG_ERROR(message, ...) Logger::get().error(message, __VA_ARGS__)
#define LOG_CRIT(message, ...) Logger::get().crit(message, __VA_ARGS__)
#define LOG_DEBUG(message, ...) Logger::get().debug(message, __VA_ARGS__)