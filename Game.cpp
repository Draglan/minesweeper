#include "Game.h"
#include "ScreenWriter.h"
#include "TileEffect.h"
#include <limits>
#include <cstring>
#include <queue>
#include <map>
#include <iostream>
#include <sstream>
#include <numeric>
#include <iomanip>

const std::string Game::defaultFont_ = "arial.ttf";

Game& Game::Inst() {
  static Game instance;
  return instance;
}

Game::Game()
  : window_("Minesweeper", defaultWindowW_, defaultWindowH_),
  done_(false),
  fps_(0.0f),
	states_(),
	tmpStates_()
{
  TextureFactory::Inst().Initialize(window_);
  ScreenWriter::Inst().Initialize(window_);

  ScreenWriter::Inst().SetCurrentFont(DefaultFontName(), DefaultPtSize());
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
	const int fpsUpdateDelay = 100;
	Uint32 fpsUpdateTimer = SDL_GetTicks() + fpsUpdateDelay;
	const float maxFrameRate = 30.0f;
  const int avgSize = 30;
  float fpsRunningAvg[avgSize] = {0.0f};
  int i = 0;

  while (!done_) {
		ticks = SDL_GetTicks() - timestamp;
		timestamp = SDL_GetTicks();

		// keep track of past N ticks for calculating avg. fps
		fpsRunningAvg[i] = ticks;
		i = (i + 1) % avgSize;

		// calculate FPS occasionally
		if (SDL_GetTicks() >= fpsUpdateTimer) {
			float denom = std::accumulate(fpsRunningAvg,
				fpsRunningAvg + avgSize, 0.0f) / avgSize;

			if (denom != 0.0f) {
				fps_ = 1000.0f / denom;
			}
			fpsUpdateTimer = SDL_GetTicks() + fpsUpdateDelay;
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

		// Destroy temporary states
		tmpStates_.clear();

		// cap framerate by sleeping off excess frame time
		ticks = SDL_GetTicks() - timestamp;

		if (ticks < 1000.0f / maxFrameRate) {
			SDL_Delay(1000.0f / maxFrameRate - ticks);
		}
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

	// draw FPS to screen in top left
	std::stringstream ss;
	ss << "FPS: " << fps_;
	
	Font* f = ScreenWriter::Inst().GetFont("arial.ttf", 14);
	
	if (f){
		ScreenWriter::Inst().Write(f, {255,255,255,255}, 0, 0, ss.str(), false);
	}
	
  window_.Present();
}

void Game::PopState() {
	if (!states_.empty()) {

		tmpStates_.emplace_back(
			std::unique_ptr<IGameState>(std::move(states_.back()))
		);

		states_.pop_back();
	}
}

void Game::ClearStates() {
	while (!states_.empty()) {
		PopState();
	}
}

IGameState* Game::GetTopState() const {
	if (!states_.empty())
		return states_.back().get();
	else
		return nullptr;
}
