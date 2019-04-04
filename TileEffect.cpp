#include "TileEffect.h"
#include "Window.h"
#include "Board.h"
#include <iostream>

TileEffect::TileEffect(Uint32 delay, Texture& t, const Rectangle& src,
  const Rectangle& dst)
  : delay_(delay), texture_(t), textureSrc_(src),
  dst_(dst), curTime_(0),
  color_({255,255,255,255}), currentAlpha_(255.0f)
{
}

void TileEffect::Update(Uint32 ticks) {
  curTime_ += ticks;
  if (curTime_ >= delay_) {
    currentAlpha_ -= alphaPerSec_ * ticks * 0.001f;

    if (currentAlpha_ < 0.0f) {
      currentAlpha_ = 0.0f;
      Delete();
    }

    color_.a = static_cast<Uint8>(currentAlpha_);
  }
}

void TileEffect::Draw(const Window& window) const {
  window.Draw(texture_, &textureSrc_, &dst_, color_.r, color_.g, color_.b,
    color_.a);

	const float aRatio = highlightAlpha_ / 255.0f;
	window.DrawFilledRect(&dst_, {255,255,255,
		static_cast<Uint8>(color_.a * aRatio)});
}
