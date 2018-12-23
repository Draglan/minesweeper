#include "MinesweeperState.h"
#include "TileEffect.h"
#include "Texture.h"
#include <queue>

MinesweeperState::MinesweeperState(int w, int h, int mineCount,
	int tilew, int tileh)
	: tileW_(tilew), tileH_(tileh), tiles_(), drawables_(),
	hud_(*this),
	board_(w, h),
	numMines_(mineCount),
	background_(*TextureFactory::Inst().GetTexture("background2.png"))
{
	Game::Inst().GetWindow().SetDimensions(
		tilew*w+tilew*3,
		tileh*h+tileh*3
	);

	// position the board in the middle of the screen
	int totalW = tileW_*w;
	int totalH = tileH_*h;
	int wndW = Game::Inst().GetWindow().Width();
	int wndH = Game::Inst().GetWindow().Height();

	bX_ = wndW / 2 - totalW / 2;
	bY_ = wndH / 2 - totalH / 2;

	// initialize the tiles
	for (int y = 0; y < board_.Height(); ++y) {
		for (int x = 0; x < board_.Width(); ++x) {
			tiles_.emplace_back(
				ClickableTile(*this, board_, x*tileW_ + bX_, y*tileH_ + bY_,
					tileW_, tileH_, x, y)
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

	hud_.Update(ticks);
}

void MinesweeperState::Draw(const Window& w) const {
	//w.ClearScreen(0, 97, 121);
	//w.ClearScreen(150,150,150);
	w.Draw(background_);

	// draw dropshadow
	Rectangle r(
		bX_ + tileW_/2, bY_ + tileH_/2,
		tileW_*board_.Width(), tileH_*board_.Height());

	w.DrawFilledRect(&r, {0,0,0,100});

	// draw border
	r = {bX_-1, bY_-1, tileW_*board_.Width()+2, tileH_*board_.Height()+2};
	w.DrawRect(&r, {0,82,81,255});

	for (auto& t : tiles_) {
		t.Draw(w);
	}

	for (auto& d : drawables_) {
		d->Draw(w);
	}

	hud_.Draw(w);
}

void MinesweeperState::ResetGame() {
  drawables_.clear();

  board_.SetDimensions(board_.Width(), board_.Height());
	hud_.StopTimer();
	hud_.ResetTime();
	SetFlagsUsed(0);
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

						Rectangle dst;
						dst.x = n[i].first*tileW_+bX_;
						dst.y = n[i].second*tileH_+bY_;
						dst.w = tileW_;
						dst.h = tileH_;

						SpawnDrawable(new TileEffect(curDelay, tile,
							src, dst));
					}
				}
			}

			curDelay += dDelay;
		}
	}
}
