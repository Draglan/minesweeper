#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <string>
#include "Texture.h"
#include "Rectangle.h"

class Window {
public:
	Window(const std::string& title, unsigned w, unsigned h);
	Window(const Window&) = delete;
	Window(Window&&);
	~Window();

	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&);

	unsigned Width() const {return width_;}
	unsigned Height() const {return height_;}

	// Drawing methods.
	//

	void ClearScreen() const {SDL_RenderClear(renderer_);}
	void ClearScreen(Uint8 r, Uint8 g, Uint8 b) const;
	void Present() const {SDL_RenderPresent(renderer_);}

	// Draw the entire texture to the entire screen.
	void Draw(const Texture& t) const {
		SDL_RenderCopy(renderer_, t, nullptr, nullptr);
	}

	// Draw a given portion of the texture to a given portion of the window.
	// If a Rectangle is left NULL, then the entire region is used.
	void Draw(const Texture& t, const Rectangle* src, const Rectangle* dst) const;

	// Draw a texture with the given hue and alpha.
	void Draw(
		const Texture& t, const Rectangle* src, const Rectangle* dst,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a
	) const;

	// Draw a texture with the given rotation and flipping.
	void Draw(const Texture& t, const Rectangle* src, const Rectangle* dst,
		double angle, SDL_RendererFlip flip) const;

	// Draw a texture with the given hue, alpha, rotation, and flipping.
	void Draw(const Texture& t, const Rectangle* src, const Rectangle* dst,
		Uint8 r, Uint8 g, Uint8 b, Uint8 a,
		double angle, SDL_RendererFlip flip) const;

	// Draw the entirety of a given texture to the given location.
	void Draw(const Texture& t, int x, int y) const;


	// Drawing primitives
	//
	void DrawFilledRect(const Rectangle* rect, SDL_Color col) const;
	void DrawRect(const Rectangle* rect, SDL_Color col) const;
	void DrawPoint(int x, int y, SDL_Color col) const;
	void DrawLine(int x1, int y1, int x2, int y2, SDL_Color col) const;



	SDL_Window* SDLWindow() const {return window_;}
	operator SDL_Window*() const {return window_;}

	SDL_Renderer* SDLRenderer() const {return renderer_;}
	operator SDL_Renderer*() const {return renderer_;}

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	unsigned width_, height_;
};

#endif /* WINDOW_H */
