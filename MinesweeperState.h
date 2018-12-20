#ifndef MINESWEEPER_STATE_H
#define MINESWEEPER_STATE_H

#include "IGameState.h"
#include "Game.h"
#include "Board.h"
#include "Drawable.h"
#include "ClickableTile.h"
#include <memory>
#include <vector>

class MinesweeperState : public IGameState {
public:
	MinesweeperState(int w, int h, int mineCount);
	virtual ~MinesweeperState() {}

	// IGameState overrides
	//
	virtual void HandleInput(const SDL_Event& ev) override;
	virtual void Update(Uint32 ticks) override;
	virtual void Draw(const Window& w) const override;

	// Get the number of mines currently set.
	int GetMineCount() const {return numMines_;}

	// Spawn a drawable. Ownership over the drawable is taken over
	// by the scene.
	void SpawnDrawable(Drawable* d) {
		drawables_.emplace_back(std::unique_ptr<Drawable>(d));
	}

	// Reset the game with a new width, height, and mine count.
	void ResetGame(int w, int h, int numMines);

	// Spawn the clear effects used when clearing tiles. This should be called
	// BEFORE Board::RevealFrom is actually called.
	void SpawnClearEffects(int tileX, int tileY, Texture& tile, const Rectangle& src);

private:
	static const int tileW_ = 32;
	static const int tileH_ = 32;

	std::vector<ClickableTile> tiles_;
	std::vector<std::unique_ptr<Drawable>> drawables_;
	Board board_;
  int numMines_;
};

#endif /* MINESWEEPER_STATE_H */
