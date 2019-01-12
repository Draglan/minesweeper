#ifndef WIN_LOSE_STATE_H
#define WIN_LOSE_STATE_H

#include "IGameState.h"
#include "Button.h"
#include "ScreenWriter.h"
#include "PopupMenu.h"

class WinLoseState : public IGameState {
public:
	WinLoseState(bool didWin);
	virtual ~WinLoseState() {}

	// IGameState overrides
	//
	virtual void HandleInput(const SDL_Event& ev) override;
	virtual void Update(Uint32 ticks) override;
	virtual void Draw(const Window& w) const override;

private:
	bool win_;
	PopupMenu menu_;
};

#endif /* WIN_LOSE_STATE_H */
