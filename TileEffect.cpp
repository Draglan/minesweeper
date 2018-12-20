#include "TileEffect.h"
#include "Window.h"
#include "Board.h"
#include <iostream>

TileEffect::TileEffect(Uint32 delay, Texture& t, const Rectangle& src,
  int x, int y)
  : delay_(delay), texture_(t), textureSrc_(src),
  curTime_(0), x_(x), y_(y),
  color_({255,255,255,255}), currentAlpha_(255.0f)
{
}

void TileEffect::Update(Uint32 ticks, Board& board) {
  curTime_ += ticks;
  if (curTime_ >= delay_) {
    currentAlpha_ -= alphaPerSec_ * ticks * 0.001f;

    if (currentAlpha_ < 0.0f) {
      currentAlpha_ = 0.0f;
      //if (!ShouldDelete())
        //std::cout << "Deleting after " << curTime_ << '\n';
      Delete();
    }

    color_.a = static_cast<Uint8>(currentAlpha_);
  }
}

void TileEffect::Draw(const Window& window, const Board& board) const {
  Rectangle dst(x_, y_, textureSrc_.w, textureSrc_.h);
  window.Draw(texture_, &textureSrc_, &dst, color_.r, color_.g, color_.b,
    color_.a);
  //window.DrawFilledRect(&dst, {255,255,255,color_.a/2});
}
