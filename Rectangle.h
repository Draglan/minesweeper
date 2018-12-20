#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL.h>

struct Rectangle {
	Rectangle() : x(0), y(0), w(0), h(0) {}

	Rectangle(int x, int y, int w, int h)
		: x(x), y(y), w(w), h(h)
	{
	}

	Rectangle(const SDL_Rect& rect)
		: x(rect.x), y(rect.y), w(rect.w), h(rect.h)
	{
	}

	Rectangle(const Rectangle&) = default;
	Rectangle(Rectangle&&) = default;

	Rectangle& operator=(const Rectangle&) = default;
	Rectangle& operator=(Rectangle&&) = default;

	operator SDL_Rect() const {return {x, y, w, h};}
	SDL_Rect GetSDLRect() const {return {x, y, w, h};}

	int x, y, w, h;
};

#endif /* RECTANGLE_H */
