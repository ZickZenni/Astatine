#pragma once

/**
 * @class DetourHook
 * @brief A class to manage function hooks based on detouring, using the MinHook library.
 */
class DetourHook {
public:
    DetourHook(void* target, void* detour);

    ~DetourHook();

    /**
     * @brief Enables the detour hook for the target function.
     *
     * This method attempts to enable the hook using the MinHook library's
     * MH_EnableHook function. If the hook cannot be enabled, an exception
     * is thrown with a descriptive error message.
     *
     * @throws std::runtime_error If enabling the hook fails.
     */
    void enable() const;

    /**
     * @brief Disables the detour hook for the target function.
     *
     * This method attempts to disable the hook using the MinHook library's
     * MH_DisableHook function. If the hook cannot be disabled, an exception
     * is thrown with a descriptive error message.
     *
     * @throws std::runtime_error If disabling the hook fails.
     */
    void disable() const;

    /**
     * @brief Retrieves the original function pointer cast to the specified type.
     *
     * This method returns the original function pointer used prior to detouring,
     * cast to the specified type T, for direct invocation or other uses.
     *
     * @tparam T The type to which the original function pointer will be cast.
     * @return The original function pointer cast to the specified type.
     */
    template<typename T>
    T get_original() const
    {
        return reinterpret_cast<T>(m_original);
    }

private:
    void* m_target;
    void* m_detour;
    void* m_original;
};