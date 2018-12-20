#include "MinesweeperState.h"
#include "TileEffect.h"
#include "Texture.h"
#include <queue>

MinesweeperState::MinesweeperState(int w, int h, int mineCount)
	: tiles_(), drawables_(), board_(w, h), numMines_(mineCount)
{
	// initialize the tiles
	for (int y = 0; y < board_.Height(); ++y) {
		for (int x = 0; x < board_.Width(); ++x) {
			tiles_.emplace_back(
				ClickableTile(*this, board_, x*tileW_, y*tileH_, tileW_, tileH_, x, y)
			);
		}
	}
}

void MinesweeperState::HandleInput(const SDL_Event& ev) {
	for (auto& t : tiles_)
		t.HandleInput(ev);

	for (auto& d : drawables_)
		d->HandleInput(ev);
}

void MinesweeperState::Update(Uint32 ticks) {
	for (auto& t : tiles_)
		t.Update(ticks);

	for (auto it = drawables_.begin(); it != drawables_.end(); ) {
		(*it)->Update(ticks);

		if ((*it)->ShouldDelete()) {
			it = drawables_.erase(it);
		}
		else {
			++it;
		}
	}
}

void MinesweeperState::Draw(const Window& w) const {
	for (auto& t : tiles_) {
		t.Draw(w);
	}

	for (auto& d : drawables_) {
		d->Draw(w);
	}
}

void MinesweeperState::ResetGame(int w, int h, int numMines) {
	tiles_.clear();
  drawables_.clear();

  board_.SetDimensions(w, h);
  numMines_ = numMines;

  for (int y = 0; y < board_.Height(); ++y) {
    for (int x = 0; x < board_.Width(); ++x) {
      tiles_.emplace_back(
        ClickableTile(*this, board_, x*tileW_, y*tileH_, tileW_, tileH_, x, y)
      );
    }
  }
}

void MinesweeperState::
SpawnClearEffects(int tileX, int tileY, Texture& tile, const Rectangle& src) {
	// Uses a BFS to spawn TileEffects in an order that is
	// pleasing to the eye...
	//

	Uint32 curDelay = 0;
	const Uint32 dDelay = 10;
	using PointT = std::pair<int,int>;

	std::queue<PointT> toVisit;
	std::map<PointT, bool> visited;
	visited[PointT(tileX, tileY)] = true;

	toVisit.emplace(PointT(tileX, tileY));

	while (!toVisit.empty()) {
		PointT node = toVisit.front();
		toVisit.pop();

		// visit neighbors if no adjacent mines
		if (board_.At(node.first, node.second).adjacentMines == 0) {
			PointT n[8] = {
				{node.first, node.second-1},
				{node.first, node.second+1},
				{node.first+1, node.second},
				{node.first-1, node.second},

				{node.first-1, node.second-1},
				{node.first-1, node.second+1},
				{node.first+1, node.second-1},
				{node.first+1, node.second+1}
			};

			for (int i = 0; i < 8; ++i) {
				if (visited.count(n[i])==0) {
					if (n[i].first >= 0 && n[i].second >= 0 &&
							n[i].first < board_.Width() && n[i].second < board_.Height() &&
							board_.At(n[i].first, n[i].second).status != Tile::REVEALED)
					{
						visited[n[i]] = true;
						toVisit.push(n[i]);

						SpawnDrawable(new TileEffect(curDelay, tile,
							src, n[i].first*tileW_, n[i].second*tileH_));
					}
				}
			}

			curDelay += dDelay;
		}
	}
}
