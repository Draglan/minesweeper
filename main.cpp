#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstring>
#include "Game.h"
#include "Board.h"

int main(int argc, char** argv) {
	try {
		Game::Inst().Run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception thrown:\n";
		std::cerr << e.what() << '\n';
	}
	return 0;
}
