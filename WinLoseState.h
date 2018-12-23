#ifndef WIN_LOSE_STATE_H
#define WIN_LOSE_STATE_H

#include "IGameState.h"
#include "Button.h"
#include "ScreenWriter.h"
#include "Button.h"

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
	Button playAgain_;
	Button mainMenu_;
};

#endif /* WIN_LOSE_STATE_H */
