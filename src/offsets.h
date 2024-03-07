#pragma once

#include <vector>

struct offsets_t
{
    std::size_t dll_code_size {};
    std::size_t dll_entrypoint {};
    std::size_t dll_image_size {};

    std::uintptr_t hook_a {};
    std::uintptr_t hook_b {};
    std::uintptr_t textures {};
    std::uintptr_t device {};
};