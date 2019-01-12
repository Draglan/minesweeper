#include "WinLoseState.h"
#include "ScreenWriter.h"
#include "Font.h"
#include "Game.h"
#include "MinesweeperState.h"
#include "MainMenuState.h"
#include <string>
#include <iostream>

static void PlayAgain();
static void MainMenu();

WinLoseState::WinLoseState(bool didWin)
	: win_(didWin),
	menu_(didWin ? "You win!" : "You lose!")
{
	menu_.AddButton("Play Again", PlayAgain);
	menu_.AddButton("Main Menu", MainMenu);
}

void WinLoseState::HandleInput(const SDL_Event& ev) {
	menu_.HandleInput(ev);
}

void WinLoseState::Update(Uint32 ticks) {
	menu_.Update(ticks);
}

void WinLoseState::Draw(const Window& w) const {
	menu_.Draw(w);
}

static void PlayAgain() {
	Game::Inst().PopState();
	MinesweeperState* s = dynamic_cast<MinesweeperState*>(
		Game::Inst().GetTopState());

	if (s) {
		s->ResetGame();
	}
}

static void MainMenu() {
	Game::Inst().ClearStates();
	Game::Inst().PushState(new MainMenuState);
}
