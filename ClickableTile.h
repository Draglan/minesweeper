#ifndef CLICKABLE_TILE_H
#define CLICKABLE_TILE_H

#include <SDL.h>
#include "Drawable.h"

class Board;
class Texture;
class Window;

class ClickableTile : public Drawable {
public:
  ClickableTile(int x, int y, unsigned width, unsigned height,
    unsigned boardX, unsigned boardY);

  virtual void HandleInput(const SDL_Event& ev, Board& board) override;
  virtual void Update(Uint32 ticks, Board& board) override;
  virtual void Draw(const Window& w, const Board& board) const override;

private:
	void OnLeftClick(const SDL_Event& ev, Board& board);
	void OnRightClick(const SDL_Event& ev, Board& board);
	void OnMouseEnter(const SDL_Event& ev, Board& board);
	void OnMouseLeave(const SDL_Event& ev, Board& board);

  int x_, y_;
  unsigned width_, height_, boardX_, boardY_;
  Texture& texture_;

  // Fade effects
  //
  bool doFadeInEffect_;
  SDL_Color fadeColor_;
  float currentAlpha_;
  const float fadeSpeed_ = 700.0f; // points per second
  const float maxAlpha_ = 100.0f;
  bool clickedInTile_;
};

#endif /* CLICKABLE_TILE_H */
