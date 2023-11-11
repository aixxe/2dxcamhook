#pragma once

#include <format>

extern "C" void log_body_fatal(const char* module, const char* fmt, ...);
extern "C" void log_body_info(const char* module, const char* fmt, ...);
extern "C" void log_body_misc(const char* module, const char* fmt, ...);
extern "C" void log_body_warning(const char* module, const char* fmt, ...);

namespace avs2::log
{
    auto constexpr module = "2dxcamhook";

    template <typename... Args> auto fatal(std::string_view fmt, Args&&... args)
        { log_body_fatal(module, "%s", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)).c_str()); }
    template <typename... Args> auto info(std::string_view fmt, Args&&... args)
        { log_body_info(module, "%s", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)).c_str()); }
    template <typename... Args> auto misc(std::string_view fmt, Args&&... args)
        { log_body_misc(module, "%s", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)).c_str()); }
    template <typename... Args> auto warning(std::string_view fmt, Args&&... args)
        { log_body_warning(module, "%s", std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)).c_str()); }
}