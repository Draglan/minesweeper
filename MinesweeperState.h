#ifndef MINESWEEPER_STATE_H
#define MINESWEEPER_STATE_H

#include "IGameState.h"
#include "Game.h"
#include "Board.h"
#include "Drawable.h"
#include "ClickableTile.h"
#include "HUD.h"
#include <memory>
#include <vector>

class MinesweeperState : public IGameState {
public:
	MinesweeperState(int w, int h, int mineCount, int tilew=32, int tileh=32);

	virtual ~MinesweeperState() {}

	// IGameState overrides
	//
	virtual void HandleInput(const SDL_Event& ev) override;
	virtual void Update(Uint32 ticks) override;
	virtual void Draw(const Window& w) const override;

	// Get the width and height of the tiles.
	//
	int GetTileWidth() const {return tileW_;}
	int GetTileHeight() const {return tileH_;}

	// Get the number of mines currently set.
	int GetMineCount() const {return numMines_;}

	// Spawn a drawable. Ownership over the drawable is taken over
	// by the scene.
	void SpawnDrawable(Drawable* d) {
		drawables_.emplace_back(std::unique_ptr<Drawable>(d));
	}

	// Reset the game.
	void ResetGame();

	// Spawn the clear effects used when clearing tiles. This should be called
	// BEFORE Board::RevealFrom is actually called.
	void SpawnClearEffects(int tileX, int tileY, Texture& tile, const Rectangle& src);

	void IncrementFlagsUsed() {++flagsUsed_;}
	void DecrementFlagsUsed() {--flagsUsed_;}
	int GetFlagsUsed() const {return flagsUsed_;}
	void SetFlagsUsed(int n) {flagsUsed_ = n;}

	// Get a reference to the HUD.
	HUD& GetHUD() {return hud_;}
	const HUD& GetHUD() const {return hud_;}

private:
	int tileW_ = 32;
	int tileH_ = 32;

	int flagsUsed_ = 0;

	int bX_ = 0;
	int bY_ = 0;

	std::vector<ClickableTile> tiles_;
	std::vector<std::unique_ptr<Drawable>> drawables_;
	HUD hud_;
	Board board_;
  int numMines_;

	Texture& background_;
};

#endif /* MINESWEEPER_STATE_H */
