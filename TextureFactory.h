#ifndef TEXTURE_FACTORY_H
#define TEXTURE_FACTORY_H

#include "Texture.h"
#include <unordered_map>
#include <string>
#include <memory>

class Game;
class Window;

class TextureFactory {
public:
  ~TextureFactory() {}
  static TextureFactory& Inst();
  void Initialize(Window& wnd) {wnd_ = &wnd;}

  Texture* GetTexture(const std::string& name);

private:
  TextureFactory();
  TextureFactory(const TextureFactory&) = delete;
  TextureFactory(TextureFactory&&) = delete;
  TextureFactory& operator=(const TextureFactory&) = delete;
  TextureFactory& operator=(TextureFactory&&) = delete;

  std::unordered_map<std::string, std::unique_ptr<Texture>> textures_;
  Window* wnd_;
};

#endif /* TEXTURE_FACTORY_H */
