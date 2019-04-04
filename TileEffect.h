#ifndef TILE_EFFECT_H
#define TILE_EFFECT_H

#include "Drawable.h"
#include "Rectangle.h"
#include <SDL.h>

class Board;
class Window;
class Texture;

class TileEffect : public Drawable {
public:
  TileEffect(Uint32 delay, Texture& t, const Rectangle& src, const Rectangle& dst);
  virtual ~TileEffect(){}

  virtual void HandleInput(const SDL_Event& ev) override {}
  virtual void Update(Uint32 ticks) override;
  virtual void Draw(const Window& window) const override;


private:
  Uint32 delay_;
  Texture& texture_;
  Rectangle textureSrc_;
	Rectangle dst_;
  Uint32 curTime_;
  SDL_Color color_;
  float currentAlpha_;
  const float alphaPerSec_ = 700.0f;
	const float highlightAlpha_ = 150.0f;
};

#endif /* TILE_EFFECT_H */
