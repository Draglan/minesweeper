#include "ScreenWriter.h"
#include "Game.h"
#include "Surface.h"

ScreenWriter& ScreenWriter::Inst() {
  static ScreenWriter instance;
  return instance;
}

Font* ScreenWriter::GetFont(const std::string& name, unsigned ptSize) {
  FontPair p(name, ptSize);

  if (fonts_.count(p) != 0) {
    return fonts_.at(p).get();
  }
  else {
    std::unique_ptr<Font> f = std::make_unique<Font>(name, ptSize);

    if (f->SDLFont()) {
      fonts_[p] = std::move(f);
      return fonts_.at(p).get();
    }
  }
  return nullptr;
}

Texture* ScreenWriter::GetText(Font* f, SDL_Color fg, const std::string& text) {
  if (f) {
    StringFontPair p(text, f);
    if (texts_.count(p) > 0) {
      return texts_[p].get();
    }
    else {
      Surface srf = TTF_RenderText_Blended(*f, text.c_str(), fg);
      std::unique_ptr<Texture> t = std::make_unique<Texture>(*wnd_, srf);
      texts_[p] = std::move(t);
      return texts_.at(p).get();
    }
  }
  return nullptr;
}

void ScreenWriter::SetCurrentFont(const std::string& name, unsigned ptSize) {
  Font* f = GetFont(name, ptSize);
  if (f) {
    currentFont_ = f;
  }
}

void ScreenWriter::Write(const std::string font, unsigned ptSize, SDL_Color fg,
  int x, int y, const std::string& text, bool cacheText)
{
  Font* f = GetFont(font, ptSize);
  Write(f, fg, x, y, text, cacheText);
}

void ScreenWriter::Write(Font* f, SDL_Color fg, int x, int y,
  const std::string& text, bool cacheText)
{
  if (f) {
    if (cacheText) {
      Texture* t = GetText(f, fg, text);

      if (t) {
        int w = static_cast<int>(t->Width());
        int h = static_cast<int>(t->Height());
        SDL_Rect dst = {x, y, w, h};
        SDL_RenderCopy(*wnd_, *t, nullptr, &dst);
      }
    }
    else {
      Surface srf = TTF_RenderText_Blended(*f, text.c_str(), fg);
      Texture t(*wnd_, srf);
      Rectangle dst(x, y, static_cast<int>(t.Width()),
        static_cast<int>(t.Height()));
      wnd_->Draw(t, nullptr, &dst);
    }
  }
}
