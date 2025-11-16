#pragma once

#include <d3d9.h>
#include <SpoutDX/SpoutDX.h>

struct textures
{
    LPDIRECT3DTEXTURE9* camera_texture;
    LPDIRECT3DTEXTURE9* preview_texture;
};

class receiver
{
    public:
        receiver() = default;
        receiver(std::shared_ptr<spoutDX>, LPDIRECT3DDEVICE9EX, std::string, textures&&);

        ~receiver();

        receiver(const receiver&) = delete;
        receiver& operator=(const receiver&) = delete;
        receiver(receiver&&) = default;
        receiver& operator=(receiver&&) = default;

        [[nodiscard]] auto name() const -> const std::string&;
        [[nodiscard]] auto active() const -> bool;
        [[nodiscard]] auto texture() const -> LPDIRECT3DTEXTURE9;

        auto connect() -> bool;
        auto update() -> bool;
    private:
        bool _active = false;
        std::string _name = {};
        LPDIRECT3DDEVICE9EX _device = nullptr;
        LPDIRECT3DTEXTURE9 _texture = nullptr;
        LPDIRECT3DTEXTURE9* _camera_target = nullptr;
        LPDIRECT3DTEXTURE9 _camera_original = nullptr;
        LPDIRECT3DTEXTURE9* _preview_target = nullptr;
        LPDIRECT3DTEXTURE9 _preview_original = nullptr;
        std::shared_ptr<spoutDX> _spout = nullptr;
};