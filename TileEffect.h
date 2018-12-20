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
  TileEffect(Uint32 delay, Texture& t, const Rectangle& src, int x, int y);
  virtual ~TileEffect(){}

  virtual void HandleInput(const SDL_Event& ev, Board& board) override {}
  virtual void Update(Uint32 ticks, Board& board) override;
  virtual void Draw(const Window& window, const Board& board) const override;


private:
  Uint32 delay_;
  Texture& texture_;
  Rectangle textureSrc_;
  Uint32 curTime_;
  int x_, y_;
  SDL_Color color_;
  float currentAlpha_;
  const float alphaPerSec_ = 700.0f;
};

#endif /* TILE_EFFECT_H */
