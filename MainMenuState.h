#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "IGameState.h"
#include "Button.h"
#include "PopupMenu.h"

class Texture;

class MainMenuState : public IGameState {
public:
	MainMenuState();

	virtual void HandleInput(const SDL_Event& ev) override;
	virtual void Update(Uint32 ticks) override;
	virtual void Draw(const Window& w) const override;

private:
	Texture& background_;
	PopupMenu menu_;
};

#endif /* MAIN_MENU_STATE_H */
