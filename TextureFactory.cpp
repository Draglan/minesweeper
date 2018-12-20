#include "TextureFactory.h"
#include "Game.h"
#include "Window.h"

TextureFactory& TextureFactory::Inst() {
  static TextureFactory instance;
  return instance;
}

TextureFactory::TextureFactory()
  : textures_(), wnd_(nullptr)
{
}

Texture* TextureFactory::GetTexture(const std::string& name) {
  if (textures_.count(name) != 0) {
    return textures_.at(name).get();
  }
  else {
    std::unique_ptr<Texture> t = std::make_unique<Texture>(
      *wnd_, name);

    if (t->SDLTexture()) {
      textures_[name] = std::move(t);
      return textures_.at(name).get();
    }
  }
  return nullptr;
}
