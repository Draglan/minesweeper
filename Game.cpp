#include "Game.h"
#include "ScreenWriter.h"
#include "TileEffect.h"
#include <cstring>
#include <queue>
#include <map>
#include <iostream>
#include <sstream>
#include <numeric>
#include <iomanip>

Game& Game::Inst() {
  static Game instance;
  return instance;
}

Game::Game()
  : window_("Minesweeper", 640, 480),
  done_(false),
  tiles_(),
  drawables_(),
  board_(20, 15),
  fps_(0.0f),
  numMines_(0)
{
  TextureFactory::Inst().Initialize(window_);
  ScreenWriter::Inst().Initialize(window_);

  ScreenWriter::Inst().SetCurrentFont("arial.ttf", 32);
  ScreenWriter::Inst().SetColor({0,0,0,255});

  for (int y = 0; y < board_.Height(); ++y) {
    for (int x = 0; x < board_.Width(); ++x) {
      tiles_.emplace_back(
        ClickableTile(x * tileW_, y * tileH_, tileW_, tileH_, x, y)
      );
    }
  }
}

Game::~Game() {}

void Game::Run() {
  SDL_Event ev;
  std::memset(&ev, 0, sizeof(ev));

  Uint32 ticks = 0;
  Uint32 timestamp = SDL_GetTicks();

  // FPS variables
  //
  const Uint32 fpsUpdateDelay = 100;
  Uint32 fpsUpdateTime = SDL_GetTicks() + fpsUpdateDelay;
  const int avgSize = 10;
  float fpsRunningAvg[avgSize] = {0.0f};
  int i = 0;

  const float maxFrameRate = 30.0f;

  while (!done_) {
    ticks = SDL_GetTicks() - timestamp;
    timestamp = SDL_GetTicks();

    // update fps every X milliseconds
    if (SDL_GetTicks() >= fpsUpdateTime) {
      i = (i + 1) % avgSize;
      fpsRunningAvg[i] = 1000.0f / ticks;
      fps_ = std::accumulate<float*, float>(fpsRunningAvg, fpsRunningAvg + avgSize, 0.0f) / avgSize;
      fpsUpdateTime = SDL_GetTicks() + fpsUpdateDelay;
    }

    // cap framerate
    if (ticks < 1.0f / maxFrameRate * 1000.0f) {
      SDL_Delay(1.0f / maxFrameRate * 1000.0f);
    }

		// handle all input queued up for this frame
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        done_ = true;
        break;
      }
      else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE) {
        ResetGame(20, 15, 30);
      }

      for (auto& t : tiles_)
        t.HandleInput(ev, board_);
    }

		// update and render game components
    Update(ticks);
    Draw();
  }
}

void Game::Update(Uint32 ticks) {
  // update the tiles
  for (auto& t : tiles_)
    t.Update(ticks, board_);

  // update other drawables
  for (auto it = drawables_.begin(); it != drawables_.end(); ) {
    (*it)->Update(ticks, board_);

    if ((*it)->ShouldDelete()) {
      it = drawables_.erase(it);
    }
    else {
      ++it;
    }
  }
}

void Game::Draw() const {
  window_.ClearScreen();

  // draw the tiles
  for (auto& t : tiles_)
    t.Draw(window_, board_);

  // draw other drawables
  for (auto& d : drawables_)
    d->Draw(window_, board_);

  // draw framerate
  std::stringstream ss;
  ss << std::setprecision(4) << fps_;
  ScreenWriter::Inst().Write(0, 0, ss.str());

  window_.Present();
}

void Game::SpawnClearEffects(int tileX, int tileY, Texture& texture,
  const Rectangle& src)
{
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

            SpawnDrawable(new TileEffect(curDelay, texture,
              src, n[i].first*tileW_, n[i].second*tileH_));
          }
        }
      }

      curDelay += dDelay;
    }
  }
}

void Game::ResetGame(int w, int h, int numMines) {
  tiles_.clear();
  drawables_.clear();

  board_.SetDimensions(w, h);
  numMines_ = numMines;

  for (int y = 0; y < board_.Height(); ++y) {
    for (int x = 0; x < board_.Width(); ++x) {
      tiles_.emplace_back(
        ClickableTile(x*tileW_, y*tileH_, tileW_, tileH_, x, y)
      );
    }
  }
}
