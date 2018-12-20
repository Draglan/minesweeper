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
  fps_(0.0f),
	states_()
{
  TextureFactory::Inst().Initialize(window_);
  ScreenWriter::Inst().Initialize(window_);

  ScreenWriter::Inst().SetCurrentFont("arial.ttf", 32);
  ScreenWriter::Inst().SetColor({0,0,0,255});
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

      fps_ = std::accumulate<float*, float>(
				fpsRunningAvg, fpsRunningAvg + avgSize, 0.0f) / avgSize;

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

			// send input to the top state
			IGameState* s = GetTopState();
			if (s) {
				s->HandleInput(ev);
			}
    }

		// update and render game states
    Update(ticks);
    Draw();
  }
}

void Game::Update(Uint32 ticks) {
	// Update the top state
	IGameState* s = GetTopState();
	if (s) {
		s->Update(ticks);
	}
}

void Game::Draw() const {
  window_.ClearScreen();

	// draw all of the active states
	for (auto& s : states_)
		s->Draw(window_);

  // draw framerate
  std::stringstream ss;
  ss << std::setprecision(3) << fps_;
  ScreenWriter::Inst().Write(0, 0, ss.str());

  window_.Present();
}

void Game::PopState() {
	if (!states_.empty())
		states_.pop_back();
}

IGameState* Game::GetTopState() const {
	if (!states_.empty())
		return states_.back().get();
	else
		return nullptr;
}
