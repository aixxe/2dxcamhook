#include <safetyhook.hpp>
#include "receiver.h"

/* constants */
auto constexpr name_camera_a = "Camera A";
auto constexpr name_camera_b = "Camera B";
auto constexpr game_dll_name = {"bm2dx.dll", "bm2dx_omni.dll"};

/* offsets */
extern std::ptrdiff_t addr_hook_a;
extern std::ptrdiff_t addr_hook_b;
extern std::ptrdiff_t addr_textures;
extern std::ptrdiff_t addr_device_offset;

/* app context */
auto static recv_a = receiver();
auto static recv_b = receiver();
auto static init_flag = std::once_flag {};
auto static spout = std::shared_ptr<spoutDX> {};

/* game context */
auto static bm2dx = PBYTE {};

/* game hooks */
auto static hook_a = SafetyHookInline {};
auto static hook_b = SafetyHookInline {};

/* entrypoint */
auto DllMain(HMODULE, unsigned long reason, void*)
{
    if (reason != DLL_PROCESS_ATTACH)
        return TRUE;

    /* find game library */
    for (auto it = game_dll_name.begin(); !bm2dx && it != game_dll_name.end(); ++it)
        bm2dx = PBYTE(GetModuleHandleA(*it));

    /* allocate spout instance */
    spout = std::make_shared<spoutDX>();

    if (!bm2dx || !spout)
        return FALSE;

    hook_a = safetyhook::create_inline(bm2dx + addr_hook_a, +[] (PBYTE a1)
    {
        std::call_once(init_flag, [&] ()
        {
            /* get pointers to the d3d9ex device & camera textures */
            auto device = *reinterpret_cast<LPDIRECT3DDEVICE9EX*>(a1 + addr_device_offset);
            auto textures = *reinterpret_cast<LPDIRECT3DTEXTURE9**>(bm2dx + addr_textures);

            recv_a = receiver(spout, device, name_camera_a, textures);
            recv_b = receiver(spout, device, name_camera_b, textures + 2);
        });

        /* maintain receiving state */
        recv_a.update();
        recv_b.update();

        /* call original function */
        return hook_a.call<void*>(a1);
    });

    hook_b = safetyhook::create_inline(bm2dx + addr_hook_b, +[] (void* a1, int idx)
    {
        /* return the appropriate camera texture */
        if (idx == 0 && recv_a.active())
            return recv_a.texture();
        else if (idx == 1 && recv_b.active())
            return recv_b.texture();

        return hook_b.call<LPDIRECT3DTEXTURE9>(a1, idx);
    });

    return TRUE;
}