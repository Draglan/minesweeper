#include "ClickableTile.h"
#include "Texture.h"
#include "TextureFactory.h"
#include "Board.h"
#include "Window.h"
#include "ScreenWriter.h"
#include "Game.h"
#include <sstream>
#include <iostream>
#include <cmath>

ClickableTile::ClickableTile(int x, int y, unsigned width, unsigned height,
  unsigned boardX, unsigned boardY)
  : x_(x), y_(y), width_(width), height_(height),
  boardX_(boardX), boardY_(boardY),
  texture_(*TextureFactory::Inst().GetTexture("tile.png")),
  doFadeInEffect_(false), fadeColor_({255,255,255,0}),
  currentAlpha_(0.0f), clickedInTile_(false)
{
}

void ClickableTile::HandleInput(const SDL_Event& ev, Board& board) {
  if (ev.type == SDL_MOUSEBUTTONUP &&
      ev.button.x >= x_ && ev.button.x < x_ + static_cast<int>(width_) &&
      ev.button.y >= y_ && ev.button.y < y_ + static_cast<int>(height_) &&
      clickedInTile_)
  {
    clickedInTile_ = false;
    fadeColor_ = {255,255,255,static_cast<Uint8>(currentAlpha_)};

    if (ev.button.button == SDL_BUTTON_LEFT) {
			OnLeftClick(ev, board);
    }
    else if (ev.button.button == SDL_BUTTON_RIGHT) {
			OnRightClick(ev, board);
    }
  }
  else if (ev.type == SDL_MOUSEBUTTONUP) {
    clickedInTile_ = false;
  }
  else if (ev.type == SDL_MOUSEMOTION) {
    Sint32 oldX = ev.motion.x - ev.motion.xrel;
    Sint32 oldY = ev.motion.y - ev.motion.yrel;

    if (ev.motion.x >= x_ && ev.motion.x < x_ + static_cast<int>(width_) &&
        ev.motion.y >= y_ && ev.motion.y < y_ + static_cast<int>(height_))
    {
      // if we're moving inside the tile, trigger fade-in
      if ((oldX < x_ || oldX >= x_ + static_cast<int>(width_) ||
          oldY < y_ || oldY >= y_ + static_cast<int>(height_)))
      {
				OnMouseEnter(ev, board);
      }
    }
    else if (ev.motion.x < x_ || ev.motion.x >= x_ + static_cast<int>(width_) ||
            ev.motion.y < y_ || ev.motion.y >= y_ + static_cast<int>(height_))
    {
			// if we're moving outside the tile, trigger fade-out
			doFadeInEffect_ = false;
			currentAlpha_ = fadeColor_.a;
    }
  }
  else if (ev.type == SDL_MOUSEBUTTONDOWN &&
          ev.button.x >= x_ && ev.button.x < x_ + static_cast<int>(width_) &&
          ev.button.y >= y_ && ev.button.y < y_ + static_cast<int>(height_))
  {
    // if we're pressing it down, change the hover color so it is darker
    currentAlpha_ = maxAlpha_;
    fadeColor_ = {0, 0, 0, static_cast<Uint8>(currentAlpha_)};

    // If the user presses the mbutton INSIDE of the tile, then we allow it
    // to be "clicked" when the mouse button is released. This prevents the user
    // from releasing the mouse button over other tiles and having them count
    // as a click.
    clickedInTile_ = true;
  }
}

void ClickableTile::Update(Uint32 ticks, Board& board) {
  if (doFadeInEffect_) {
    currentAlpha_ += fadeSpeed_ * ticks * 0.001f;
    fadeColor_.a = std::min(currentAlpha_, maxAlpha_);
  }
  else {
    currentAlpha_ -= fadeSpeed_ * ticks * 0.001f;
    fadeColor_.a = std::max(currentAlpha_, 0.0f);
  }
}

void ClickableTile::Draw(const Window& w, const Board& board) const {
  Tile::Status s = board.At(boardX_, boardY_).status;
  Rectangle src;
  Rectangle dst(x_, y_, width_, height_);

  switch (s) {
    case Tile::HIDDEN: src = {0, 0, 32, 32}; break;
    case Tile::REVEALED:
      if (!board.At(boardX_, boardY_).hasMine)
        src = {32, 0, 32, 32};
      else
        src = {128, 0, 32, 32};
      break;
    case Tile::MARKED: src =  {64, 0, 32, 32}; break;
    case Tile::QMARK: src = {96, 0, 32, 32}; break;
  }

  w.Draw(texture_, &src, &dst);

  // draw fade effect
  if (s != Tile::REVEALED)
    w.DrawFilledRect(&dst, fadeColor_);

  // Draw adjacent mine count
  if (s == Tile::REVEALED && board.At(boardX_, boardY_).adjacentMines > 0 &&
      !board.At(boardX_, boardY_).hasMine)
  {
    std::stringstream ss;
    ss << board.At(boardX_, boardY_).adjacentMines;
    ScreenWriter::Inst().Write(x_ + 8, y_, ss.str(), true);
  }
}

void ClickableTile::OnLeftClick(const SDL_Event& ev, Board& board) {
	if (!board.IsInitialized()) {
		// if clicking for the first time, initialize the board
		board.Initialize(boardX_, boardY_, Game::Inst().GetMineCount());
		Game::Inst().SpawnClearEffects(boardX_, boardY_, texture_,
			{0, 0, 32, 32});

		board.RevealFrom(boardX_, boardY_);
	}
	else {
		if (!board.At(boardX_, boardY_).hasMine &&
				board.At(boardX_, boardY_).status != Tile::REVEALED)
		{
			Game::Inst().SpawnClearEffects(boardX_, boardY_, texture_,
				{0, 0, 32, 32});

			board.RevealFrom(boardX_, boardY_);
		}
		else if (board.At(boardX_, boardY_).hasMine) {
			// player hit a mine: reveal all mine tiles and lose the game
			for (int y = 0; y < board.Height(); ++y) {
				for (int x = 0; x < board.Width(); ++x) {
					if (board.At(x, y).hasMine)
						board.At(x, y).status = Tile::REVEALED;
				}
			}
		}
	}
}

void ClickableTile::OnRightClick(const SDL_Event& ev, Board& board) {
	// toggle flag or question mark
	//
	Tile::Status s = board.At(boardX_, boardY_).status;
	switch (s) {
		case Tile::HIDDEN:
			board.At(boardX_, boardY_).status = Tile::MARKED;
			break;

		case Tile::MARKED:
			board.At(boardX_, boardY_).status = Tile::QMARK;
			break;

		case Tile::QMARK:
			board.At(boardX_, boardY_).status = Tile::HIDDEN;
			break;

		default: break; // silence warnings
	}
}

void ClickableTile::OnMouseEnter(const SDL_Event& ev, Board& board) {
	doFadeInEffect_ = true;
	if (!clickedInTile_)
		fadeColor_ = {255,255,255,0};
	else
		fadeColor_ = {0,0,0,0};

	currentAlpha_ = 50.0f;
}

void ClickableTile::OnMouseLeave(const SDL_Event& ev, Board& board) {

}
