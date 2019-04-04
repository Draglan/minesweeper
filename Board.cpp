#include "Board.h"
#include <random>
#include <ctime>
#include <iostream>

Board::Board()
  : width_(0), height_(0), numMines_(0),
  board_(nullptr),isInitialized_(false),revealedTiles_(0)
{
}

Board::Board(int width, int height)
  : width_(width), height_(height), numMines_(0),
  board_(nullptr), isInitialized_(false), revealedTiles_(0)
{
  board_ = std::make_unique<Tile[]>(width*height);
}

Tile& Board::At(int x, int y) {
  if (x >= width_ || y >= height_)
    throw std::out_of_range("Board::At");
  return board_[y * width_ + x];
}

const Tile& Board::At(int x, int y) const {
  if (x >= width_ || y >= height_)
    throw std::out_of_range("Board::At");
  return board_[y * width_ + x];
}

void Board::SetDimensions(int w, int h) {
  board_ = std::make_unique<Tile[]>(w*h);
  width_ = w;
  height_ = h;
  isInitialized_ = false;
	revealedTiles_ = 0;
}

void Board::Initialize(int startX, int startY, int numMines) {
  isInitialized_ = true;
  numMines_ = numMines;
	revealedTiles_ = 0;

  // reset all of the tiles
  for (int i = 0; i < width_*height_; ++i) {
    board_[i].status = Tile::HIDDEN;
    board_[i].hasMine = false;
    board_[i].adjacentMines = 0;
  }

  // place numMines mines anywhere except the start location
  std::default_random_engine re(time(nullptr));
  std::uniform_int_distribution<int> widthDist(0, width_ - 1);
  std::uniform_int_distribution<int> heightDist(0, height_ - 1);

  std::map<std::pair<int,int>, bool> visited;
  for (int i = 0; i < numMines_; ) {
    int x = widthDist(re);
    int y = heightDist(re);

    if ((x != startX || y != startY) && visited.count({x, y}) == 0) {
      At(x, y).hasMine = true;
      visited[{x, y}] = true;
      ++i;

      // increment adjacent mine counts
      for (int k = y - 1; k <= y + 1; ++k) {
        for (int j = x - 1; j <= x + 1; ++j) {
          if (k >= 0 && j >= 0 && k < Height() && j < Width() && !(j==x && k==y)) {
            ++At(j, k).adjacentMines;
          }
        }
      }
    }
  }
}

int Board::RevealFrom(int x, int y) {
  if (x >= width_ || y >= height_ || At(x, y).hasMine ||
      At(x, y).status == Tile::REVEALED)
    return 0;

  At(x, y).status = Tile::REVEALED;
	++revealedTiles_;

  if (At(x, y).adjacentMines != 0)
    return 1;

	int c=0;

  for (int i = y - 1; i <= y + 1; ++i) {
    for (int j = x - 1; j <= x + 1; ++j) {
      if (j >= 0 && i >= 0 && (j != x || i != y)) {
        c += RevealFrom(j, i);
      }
    }
  }

	return c;
}
