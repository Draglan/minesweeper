#ifndef SCREEN_WRITER_H
#define SCREEN_WRITER_H

#include "Font.h"
#include <map>
#include <memory>
#include <string>
#include "Texture.h"

class Window;
class Game;

class ScreenWriter {
public:
  static ScreenWriter& Inst();
  ~ScreenWriter() {}

  void Initialize(Window& wnd) {wnd_ = &wnd;}

  Font* GetFont(const std::string& name, unsigned ptSize);
  Texture* GetText(Font* f, SDL_Color fg, const std::string& text);

  void SetCurrentFont(const std::string& name, unsigned ptSize);
  Font* GetCurrentFont() const {return currentFont_;}

  void SetColor(const SDL_Color& col) {color_ = col;}
  const SDL_Color& GetColor() const {return color_;}

  void Write(int x, int y, const std::string& text, bool cacheText = false) {
    Write(currentFont_, color_, x, y, text, cacheText);
  }

  void Write(const std::string font, unsigned ptSize, SDL_Color fg,
    int x, int y, const std::string& text, bool cacheText = false);

  void Write(Font* f, SDL_Color fg, int x, int y, const std::string& text,
    bool cacheText = false);

private:
  using FontPair = std::pair<std::string, unsigned>;
  using StringFontPair = std::pair<std::string, Font*>;

  ScreenWriter() : fonts_(), texts_(), currentFont_(nullptr),
    color_({255,255,255,255}), wnd_(nullptr) {}

  ScreenWriter(const ScreenWriter&) = delete;
  ScreenWriter(ScreenWriter&&) = delete;
  ScreenWriter& operator=(const ScreenWriter&) = delete;
  ScreenWriter& operator=(ScreenWriter&&) = delete;

  std::map<FontPair, std::unique_ptr<Font>> fonts_;
  std::map<StringFontPair, std::unique_ptr<Texture>> texts_;
  Font* currentFont_;
  SDL_Color color_;
  Window* wnd_;
};

#endif /* SCREEN_WRITER_H */
