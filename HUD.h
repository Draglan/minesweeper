#ifndef HUD_H
#define HUD_H

#include "Texture.h"
#include "Drawable.h"
#include <SDL.h>

class Font;
class MinesweeperState;

class HUD : public Drawable {
public:
	HUD(MinesweeperState& s);
	virtual ~HUD() {}

	virtual void HandleInput(const SDL_Event& ev) {}
  virtual void Update(Uint32 ticks) override;
  virtual void Draw(const Window& w) const override;

	void ResetTime() {time_ = 0; timer_ = 0;}
	void StartTimer() {doTimer_ = true;}
	void StopTimer() {doTimer_ = false;}

private:
	void DrawTimer(Font* f, const Window& w) const;
	void DrawMineCount(Font* f, const Window& w) const;

	MinesweeperState& state_;

	Texture& mine_;
	Texture& clock_;

	bool doTimer_;
	Uint32 time_; // time in seconds
	Uint32 timer_; // to keep track of passing ticks
};

#endif /* HUD_H */
