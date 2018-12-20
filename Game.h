#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "TextureFactory.h"
#include "ClickableTile.h"
#include "Board.h"
#include "IGameState.h"
#include <vector>
#include <memory>

class Game {
public:
  ~Game();
  static Game& Inst();

  void Run();
  void Quit() {done_ = true;}

	// Push a state onto the state stack.
	void PushState(IGameState* state) {
		states_.emplace_back(std::unique_ptr<IGameState>(state));
	}

	// Pop the top of the state stack off.
	void PopState();

	// Get the state on the top of the stack. Returns nullptr
	// if there are no states on the stack.
	IGameState* GetTopState() const;

	// Clear all states from the state stack.
	void ClearStates() {states_.clear();}

	// Return the game window.
	//
  Window& GetWindow() {return window_;}
  const Window& GetWindow() const {return window_;}

private:
  Game();
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;

  void Update(Uint32 ticks);
  void Draw() const;

  Window window_;
  bool done_;
  float fps_;

	std::vector<std::unique_ptr<IGameState>> states_;
};

#endif /* GAME_H */
