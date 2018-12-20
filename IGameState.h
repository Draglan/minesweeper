#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H

#include <SDL.h>
#include "Window.h"

class IGameState {
public:
	virtual ~IGameState() {}

	virtual void HandleInput(const SDL_Event& ev) = 0;
	virtual void Update(Uint32 ticks) = 0;
	virtual void Draw(const Window& w) const = 0;
};

#endif /* I_GAME_STATE_H */
