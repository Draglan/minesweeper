#include "MainMenuState.h"
#include "ScreenWriter.h"
#include "Game.h"
#include "TextureFactory.h"
#include "MinesweeperState.h"
#include <iostream>

static void PlayEasy();
static void PlayMedium();
static void PlayHard();
static void Quit();

MainMenuState::MainMenuState()
	: background_(*TextureFactory::Inst().GetTexture("background2.png")),
	menu_("Minesweeper")
{
	Game::Inst().GetWindow().SetDimensions(
		Game::Inst().DefaultWindowWidth(),
		Game::Inst().DefaultWindowHeight()
	);

	menu_.AddButton("Easy (10x10, 10 mines)", PlayEasy);
	menu_.AddButton("Medium (16x16, 40 mines)", PlayMedium);
	menu_.AddButton("Hard (30x16, 99 mines)", PlayHard);
	menu_.AddButton("Quit", Quit);
}

void MainMenuState::HandleInput(const SDL_Event& ev) {
	menu_.HandleInput(ev);
}

void MainMenuState::Update(Uint32 ticks) {
	menu_.Update(ticks);
}

void MainMenuState::Draw(const Window& w) const {
	w.Draw(background_);
	menu_.Draw(w);
}


static void PlayEasy() {
	Game::Inst().ClearStates();
	Game::Inst().PushState(new MinesweeperState(10, 10, 10, 32, 32));
}

static void PlayMedium() {
	Game::Inst().ClearStates();
	Game::Inst().PushState(new MinesweeperState(16, 16, 40, 32, 32));
}

static void PlayHard() {
	Game::Inst().ClearStates();
	Game::Inst().PushState(new MinesweeperState(30, 16, 99, 32, 32));
}

static void Quit() {
	Game::Inst().Quit();
}
