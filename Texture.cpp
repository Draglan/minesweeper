#include "Texture.h"
#include "Surface.h"
#include "Window.h"
#include <SDL_image.h>
#include <iostream>

Texture::Texture(const Window& wnd, const std::string& file)
	: texture_(nullptr), width_(0), height_(0)
{
	Surface srf = IMG_Load(file.c_str());
	width_ = srf.Width();
	height_ = srf.Height();

	texture_ = SDL_CreateTextureFromSurface(wnd.SDLRenderer(), srf);

	if (!texture_) {
		std::cerr << "Texture::Texture(): " << SDL_GetError() << std::endl;
	}
	else {
		SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
	}
}

Texture::Texture(const Window& wnd, const Surface& surface)
	: texture_(nullptr), width_(surface.Width()), height_(surface.Height())
{
	texture_ = SDL_CreateTextureFromSurface(wnd.SDLRenderer(), surface);

	if (!texture_) {
		std::cerr << "Texture::Texture(): " << SDL_GetError() << std::endl;
	}
	else {
		SDL_SetTextureBlendMode(texture_, SDL_BLENDMODE_BLEND);
	}
}

Texture::Texture(Texture&& other)
	: texture_(other.texture_), width_(other.Width()), height_(other.Height())
{
	other.texture_ = nullptr;
}

Texture& Texture::operator=(Texture&& other) {
	if (this != &other) {
		SDL_DestroyTexture(texture_);
		texture_ = other.texture_;
		other.texture_ = nullptr;
	}
	return *this;
}

void Texture::GetColorMod(Uint8& r, Uint8& g, Uint8& b) const {
	SDL_GetTextureColorMod(texture_, &r, &g, &b);
}

void Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(texture_, r, g, b);
}

void Texture::GetAlphaMod(Uint8& a) const {
	SDL_GetTextureAlphaMod(texture_, &a);
}

void Texture::SetColorMod(Uint8 a) {
	SDL_SetTextureAlphaMod(texture_, a);
}
