#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstring>
#include "Game.h"
#include "Board.h"
#include "MinesweeperState.h"

int main(int argc, char** argv) {
	try {
		Game::Inst().PushState(new MinesweeperState(29, 14, 10, 32, 32));
		Game::Inst().Run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception thrown:\n";
		std::cerr << e.what() << '\n';
	}
	return 0;
}
