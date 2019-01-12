#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "TextureFactory.h"
#include "ClickableTile.h"
#include "Board.h"
#include "IGameState.h"
#include <vector>
#include <memory>
#include <string>

class Game {
public:
  ~Game();
  static Game& Inst();

  void Run();
  void Quit() {done_ = true;}

	constexpr int DefaultWindowWidth() const {return defaultWindowW_;}
	constexpr int DefaultWindowHeight() const {return defaultWindowH_;}
	constexpr const std::string& DefaultFontName() const {return defaultFont_;}
	constexpr int DefaultPtSize() const {return defaultPtSize_;}

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
	void ClearStates();

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
  bool done_; // if true, the game will exit
  float fps_;

	// Default settings
	//
	const int defaultWindowW_ = 640;
	const int defaultWindowH_ = 480;
	const std::string defaultFont_ = "arial.ttf";
	const int defaultPtSize_ = 32;

	std::vector<std::unique_ptr<IGameState>> states_;

	// Holds states that have been popped off of the state stack.
	// They are held in memory until the end of the update cycle and then
	// destroyed (aka cleared from this vector). This is done so that states
	// aren't erased in the middle of their own method calls.
	std::vector<std::unique_ptr<IGameState>> tmpStates_;
};

#endif /* GAME_H */
