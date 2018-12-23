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
	const std::string& Name() const {return name_;}

	void SizeText(const std::string& text, int* w, int* h) {
		TTF_SizeText(font_, text.c_str(), w, h);
	}
	void SizeText(char ch, int* w, int* h);

  TTF_Font* SDLFont() const {return font_;}
  operator TTF_Font*() const {return font_;}

private:
  TTF_Font* font_;
  unsigned ptSize_;
	std::string name_;
  static unsigned fontCount_;
};

#endif /* FONT_H */
