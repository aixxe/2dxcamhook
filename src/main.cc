#include <versions.h>
#include <safetyhook.hpp>
#include "receiver.h"

/* constants */
auto constexpr name_camera_a = "Camera A";
auto constexpr name_camera_b = "Camera B";
auto constexpr game_dll_name = {"bm2dx.dll", "bm2dx_omni.dll"};

/* app context */
auto static recv_a = receiver {};
auto static recv_b = receiver {};
auto static init = std::once_flag {};
auto static spout = std::shared_ptr<spoutDX> {};

/* game context */
auto static bm2dx = PBYTE {};
auto static offsets = offsets_t {};

/* game hooks */
auto static hook_a = SafetyHookInline {};
auto static hook_b = SafetyHookInline {};

/* entrypoint */
auto DllMain(void*, unsigned long reason, void*)
{
    if (reason != DLL_PROCESS_ATTACH)
        return TRUE;

    /* find game library */
    for (auto it = game_dll_name.begin(); !bm2dx && it != game_dll_name.end(); ++it)
        bm2dx = reinterpret_cast<PBYTE>(GetModuleHandleA(*it));

    /* allocate spout instance */
    spout = std::make_shared<spoutDX>();

    if (!bm2dx || !spout)
        return FALSE;

    /* determine game version */
    auto const dos = reinterpret_cast<PIMAGE_DOS_HEADER>(bm2dx);
    auto const nt = reinterpret_cast<PIMAGE_NT_HEADERS>(bm2dx + dos->e_lfanew);
    auto const it = std::ranges::find_if(versions, [&] (auto const& info)
    {
        return info.dll_code_size == nt->OptionalHeader.SizeOfCode
            && info.dll_entrypoint == nt->OptionalHeader.AddressOfEntryPoint
            && info.dll_image_size == nt->OptionalHeader.SizeOfImage;
    });

    if (it == versions.end())
        return FALSE;

    offsets = *it;

    /* install hooks */
    hook_a = safetyhook::create_inline(bm2dx + offsets.hook_a, +[] (PBYTE a1)
    {
        std::call_once(init, [&]
        {
            /* get pointers to the d3d9ex device & camera textures */
            auto const device = *reinterpret_cast<LPDIRECT3DDEVICE9EX*>(a1 + offsets.device);
            auto const textures = *reinterpret_cast<LPDIRECT3DTEXTURE9**>(bm2dx + offsets.textures);

            recv_a = { spout, device, name_camera_a, textures };
            recv_b = { spout, device, name_camera_b, textures + 2 };
        });

        /* maintain receiving state */
        recv_a.update();
        recv_b.update();

        /* call original function */
        return hook_a.call<void*>(a1);
    });

    hook_b = safetyhook::create_inline(bm2dx + offsets.hook_b, +[] (void* a1, int idx)
    {
        /* return the appropriate camera texture */
        if (idx == 0 && recv_a.active())
            return recv_a.texture();
        if (idx == 1 && recv_b.active())
            return recv_b.texture();

        return hook_b.call<LPDIRECT3DTEXTURE9>(a1, idx);
    });

    return TRUE;
}