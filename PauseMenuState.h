#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "IGameState.h"
#include "PopupMenu.h"

class PauseMenuState : public IGameState {
public:
	PauseMenuState();

	virtual void HandleInput(const SDL_Event& ev) override;
	virtual void Update(Uint32 ticks) override;
	virtual void Draw(const Window& w) const override;

private:
	PopupMenu menu_;
};

#endif /* PAUSE_MENU_H */
