#pragma once

#include <format>

extern "C"
{
    void log_body_fatal(const char* module, const char* fmt, ...);
    void log_body_info(const char* module, const char* fmt, ...);
    void log_body_misc(const char* module, const char* fmt, ...);
    void log_body_warning(const char* module, const char* fmt, ...);
}

namespace avs2::log
{
    auto constexpr module = "2dxcamhook";

    template <typename... Args> auto constexpr fatal(std::format_string<Args...> fmt, Args&&... args)
        { log_body_fatal(module, "%s", std::format(fmt, std::forward<Args>(args)...).c_str()); }
    template <typename... Args> auto constexpr info(std::format_string<Args...> fmt, Args&&... args)
        { log_body_info(module, "%s", std::format(fmt, std::forward<Args>(args)...).c_str()); }
    template <typename... Args> auto constexpr misc(std::format_string<Args...> fmt, Args&&... args)
        { log_body_misc(module, "%s", std::format(fmt, std::forward<Args>(args)...).c_str()); }
    template <typename... Args> auto constexpr warning(std::format_string<Args...> fmt, Args&&... args)
        { log_body_warning(module, "%s", std::format(fmt, std::forward<Args>(args)...).c_str()); }
}