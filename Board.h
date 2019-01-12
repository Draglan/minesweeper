#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include <memory>
#include <map>

struct Tile {
  enum Status {HIDDEN, REVEALED, MARKED, QMARK};

  bool hasMine = false;
  int adjacentMines = 0;
  Status status = HIDDEN;
};

class Board {
public:
  Board();
  Board(int width, int height);
  Board(const Board&) = delete;
  Board(Board&&) = default;
  Board& operator=(const Board&) = delete;
  Board& operator=(Board&&) = default;

  Tile& At(int x, int y);
  const Tile& At(int x, int y) const;

  int Width() const {return width_;}
  int Height() const {return height_;}
  int MineCount() const {return numMines_;}
	int RevealedTiles() const {return revealedTiles_;}

  void SetDimensions(int w, int h);
  bool IsInitialized() const {return isInitialized_;}

  void Initialize(int startX, int startY, int numMines);
  int RevealFrom(int x, int y);

private:
  int width_, height_, numMines_;
  std::unique_ptr<Tile[]> board_;
  bool isInitialized_;
	int revealedTiles_;
};

#endif /* MINESWEEPER_BOARD_H */
