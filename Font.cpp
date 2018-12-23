#include "Font.h"
#include <stdexcept>

unsigned Font::fontCount_ = 0;

Font::Font(const std::string& name, unsigned ptSize)
  : font_(nullptr), ptSize_(ptSize), name_(name)
{
  if (fontCount_ == 0) {
    if (TTF_Init() == -1) {
      throw std::runtime_error("Failed to init SDL_ttf: " + std::string(TTF_GetError()));
    }
  }

  font_ = TTF_OpenFont(name.c_str(), static_cast<int>(ptSize));
  ++fontCount_;
}

Font::Font(Font&& other)
  : font_(other.font_), ptSize_(other.ptSize_), name_(other.name_)
{
  if (fontCount_ == 0) {
    if (TTF_Init() == -1) {
      throw std::runtime_error("Failed to init SDL_ttf: " + std::string(TTF_GetError()));
    }
  }

  other.font_ = nullptr;
  ++fontCount_;
}

Font::~Font() {
  TTF_CloseFont(font_);
  --fontCount_;

  if (fontCount_ == 0) {
    TTF_Quit();
  }
}

Font& Font::operator=(Font&& other) {
  if (this != &other) {
    TTF_CloseFont(font_);
    font_ = other.font_;
    ptSize_ = other.ptSize_;
    other.font_ = nullptr;
  }
  return *this;
}

void Font::SizeText(char ch, int* w, int* h) {
	int advance;
	TTF_GlyphMetrics(font_, ch, nullptr, nullptr, nullptr, nullptr, &advance);
	if (w) *w = advance;
	if (h) *h = TTF_FontHeight(font_);
}
