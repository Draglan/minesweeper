#include "PauseMenuState.h"
#include "MainMenuState.h"
#include "MinesweeperState.h"
#include "Game.h"

static void Resume();
static void MainMenu();
static void Restart();
static void Quit();

PauseMenuState::PauseMenuState()
	: menu_("Game Paused")
{
	menu_.AddButton("Resume", Resume);
	menu_.AddButton("Main Menu", MainMenu);
	menu_.AddButton("Restart", Restart);
	menu_.AddButton("Quit", Quit);
}

void PauseMenuState::HandleInput(const SDL_Event& ev) {
	menu_.HandleInput(ev);

	// Resume on ESC
	if (ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_ESCAPE) {
		Resume();
	}
}

void PauseMenuState::Update(Uint32 ticks) {
	menu_.Update(ticks);
}

void PauseMenuState::Draw(const Window& w) const {
	menu_.Draw(w);
}

static void Resume() {
	Game::Inst().PopState();
}

static void MainMenu() {
	Game::Inst().ClearStates();
	Game::Inst().PushState(new MainMenuState);
}

static void Restart() {
	Game::Inst().PopState();
	MinesweeperState* s = dynamic_cast<MinesweeperState*>(
		Game::Inst().GetTopState()
	);

	if (s) {
		s->ResetGame();
	}
}

static void Quit() {
	Game::Inst().Quit();
}
