#include "MainMenuState.h"
#include "ScreenWriter.h"
#include "Game.h"
#include "TextureFactory.h"
#include <iostream>

MainMenuState::MainMenuState()
	: background_(*TextureFactory::Inst().GetTexture("background2.png"))
{
	Game::Inst().GetWindow().SetDimensions(
		Game::Inst().DefaultWindowWidth(),
		Game::Inst().DefaultWindowHeight()
	);
}

void MainMenuState::HandleInput(const SDL_Event& ev) {

}

void MainMenuState::Update(Uint32 ticks) {

}

void MainMenuState::Draw(const Window& w) const {
	w.Draw(background_);
}
