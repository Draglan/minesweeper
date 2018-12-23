#include "WinLoseState.h"
#include "ScreenWriter.h"
#include "Font.h"
#include "Game.h"
#include "MinesweeperState.h"
#include "MainMenuState.h"
#include <string>

static void PlayAgain();
static void MainMenu();

WinLoseState::WinLoseState(bool didWin)
	: win_(didWin),
	playAgain_(
		"Play Again",
		ScreenWriter::Inst().GetFont("arial.ttf", 24),
		0, 0,
		PlayAgain
	),
	mainMenu_(
		"Main Menu",
		ScreenWriter::Inst().GetFont("arial.ttf", 24),
		0, 0,
		MainMenu
	)
{
	// Set the positions of the buttons
	int ww = Game::Inst().GetWindow().Width();
	int wh = Game::Inst().GetWindow().Height();
	playAgain_.SetPosition(ww/2 - playAgain_.Width()/2,
		wh/2 - playAgain_.Height()/2 + 32);

	mainMenu_.SetPosition(ww/2 - mainMenu_.Width()/2,
		wh/2 - mainMenu_.Height()/2 + 64);

}

void WinLoseState::HandleInput(const SDL_Event& ev) {
	playAgain_.HandleInput(ev);
	mainMenu_.HandleInput(ev);
}

void WinLoseState::Update(Uint32 ticks) {
	playAgain_.Update(ticks);
	mainMenu_.Update(ticks);
}

void WinLoseState::Draw(const Window& w) const {
	std::string text;
	if (win_) text = "You won!";
	else text = "You lost!";

	// Get screenwriter and a font
	ScreenWriter& sw = ScreenWriter::Inst();
	Font* f = sw.GetFont("arial.ttf", 32);
	if (!f) return;

	// Draw a bar in middle of screen
	Rectangle r(0,
		w.Height() / 2 - f->PtSize()/2,
		w.Width(),
		f->PtSize() + playAgain_.Height() + mainMenu_.Height() + 8);
	w.DrawFilledRect(&r, {0,0,0,150});

	// Draw the text
	int tw;
	f->SizeText(text, &tw, nullptr);
	sw.Write(f, {255,255,255,255}, w.Width()/2-tw/2, w.Height()/2-f->PtSize()/2,
		text, true);

	playAgain_.Draw(w);
	mainMenu_.Draw(w);
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
	Game::Inst().PopState();
	Game::Inst().PopState();
	Game::Inst().PushState(new MainMenuState);
}
