#pragma once

namespace Camera
{
    struct PlayVideoCamera
    {
        auto texture(const std::uintptr_t offset) -> IDirect3DTexture9**
        {
            // offset to AfpTextureEx* from PlayVideoCamera varies between games...
            auto const afp_texture = *reinterpret_cast<std::uint8_t**>
                (reinterpret_cast<std::uint8_t*>(this) + offset);

            // ...but it's always +0x8 from there to get to its IDirect3DTexture9*
            return reinterpret_cast<IDirect3DTexture9**>(afp_texture + 0x8);
        }
    };

    struct CCameraManager2
    {
        using camera_pointers = struct
        {
            /* 0x0000 */ PlayVideoCamera* a;
            /* 0x0008 */ PlayVideoCamera* b;
        };

        /* 0x0000 */ char pad_0000[8];
        /* 0x0008 */ camera_pointers* cameras;
    };
}