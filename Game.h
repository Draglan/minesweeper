#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "TextureFactory.h"
#include "ClickableTile.h"
#include "Board.h"
#include "Drawable.h"
#include <vector>
#include <memory>

class Game {
public:
  ~Game();
  static Game& Inst();

  void Run();
  void Quit() {done_ = true;}

  Window& GetWindow() {return window_;}
  const Window& GetWindow() const {return window_;}

  int GetMineCount() const {return numMines_;}

  void SpawnDrawable(Drawable* d) {
    drawables_.emplace_back(std::unique_ptr<Drawable>(d));
  }

  void ResetGame(int w, int h, int numMines);

  void SpawnClearEffects(int tileX, int tileY, Texture& tile, const Rectangle& src);

private:
  Game();
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;

  void Update(Uint32 ticks);
  void Draw() const;

  static const int tileW_ = 32;
  static const int tileH_ = 32;

  Window window_;
  bool done_;
  std::vector<ClickableTile> tiles_;
  std::vector<std::unique_ptr<Drawable>> drawables_;
  Board board_;
  float fps_;
  int numMines_;
};

#endif /* GAME_H */
