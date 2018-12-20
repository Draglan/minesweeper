#ifndef CLICKABLE_TILE_H
#define CLICKABLE_TILE_H

#include <SDL.h>
#include "Drawable.h"

class MinesweeperState;
class Board;
class Texture;
class Window;

class ClickableTile : public Drawable {
public:
  ClickableTile(MinesweeperState& s, Board& board,
		int x, int y, unsigned width, unsigned height,
    unsigned boardX, unsigned boardY);

  virtual void HandleInput(const SDL_Event& ev) override;
  virtual void Update(Uint32 ticks) override;
  virtual void Draw(const Window& w) const override;

private:
	void OnLeftClick(const SDL_Event& ev);
	void OnRightClick(const SDL_Event& ev);
	void OnMouseEnter(const SDL_Event& ev);
	void OnMouseLeave(const SDL_Event& ev);

	MinesweeperState& state_;
	Board& board_;

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
