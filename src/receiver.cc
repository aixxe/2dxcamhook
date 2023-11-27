#include "receiver.h"
#include "avs2-log.h"

receiver::receiver(std::shared_ptr<spoutDX> spout, LPDIRECT3DDEVICE9EX device, std::string name, LPDIRECT3DTEXTURE9* target):
    _spout(std::move(spout)), _device(device), _name(std::move(name)), _target(target), _original(*target) {}

receiver::~receiver()
{
    if (_target)
        *_target = _original;

    if (_texture)
        _texture->Release();
}

auto receiver::name() const -> const std::string&
    { return _name; }

auto receiver::active() const -> bool
    { return _active; }

auto receiver::texture() const -> LPDIRECT3DTEXTURE9
    { return _texture; }

auto receiver::connect() -> bool
{
    if (!_spout->sendernames.FindSenderName(_name.c_str()))
        return false;

    auto width = 0U;
    auto height = 0U;
    auto handle = HANDLE {};
    auto format = DWORD {};

    if (!_spout->GetSenderInfo(_name.c_str(), width, height, handle, format))
        return false;

    avs2::log::misc("creating {}x{} texture for sender '{}' using shared handle {:X}",
        width, height, _name, std::uintptr_t(handle));

    auto const hr = _device->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &_texture, &handle);

    if (FAILED(hr))
    {
        avs2::log::warning("failed to create texture (hr={}, sender={})", hr, _name);
        return false;
    }

    avs2::log::info("receiving from sender '{}'", _name);

    _active = true;
    *_target = _texture;

    return true;
}

auto receiver::update() -> bool
{
    if (!active())
        return connect();

    if (_spout->sendernames.FindSenderName(_name.c_str()))
        return true;

    avs2::log::warning("lost sender '{}'", _name);

    _active = false;
    *_target = _original;

    _texture->Release();
    _texture = nullptr;

    return false;
}