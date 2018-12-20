#ifndef FONT_H
#define FONT_H

#include <SDL_ttf.h>
#include <string>

class Font {
public:
  Font(const std::string& name, unsigned ptSize);
  Font(const Font&) = delete;
  Font(Font&& other);
  Font& operator=(const Font&) = delete;
  Font& operator=(Font&& other);
  ~Font();

  unsigned PtSize() const {return ptSize_;}

  TTF_Font* SDLFont() const {return font_;}
  operator TTF_Font*() const {return font_;}

private:
  TTF_Font* font_;
  unsigned ptSize_;
  static unsigned fontCount_;
};

#endif /* FONT_H */
