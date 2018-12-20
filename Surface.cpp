#include "Surface.h"
#include "SDL_image.h"
#include <iostream>

Surface::Surface(unsigned w, unsigned h)
	: surface_(nullptr)
{
	surface_ = SDL_CreateRGBSurface(
		0,
		w, h, 32,
		RGBAMasks::rmask,
		RGBAMasks::gmask,
		RGBAMasks::bmask,
		RGBAMasks::amask
	);

	if (!surface_) {
		std::cerr << "Surface::Surface(): " << SDL_GetError()
 			<< std::endl;
	}
}

Surface::Surface(const std::string file)
	: surface_(IMG_Load(file.c_str()))
{
	if (!surface_) {
		std::cerr << "Surface::Surface(): " << SDL_GetError()
			<< std::endl;
	}
}

Surface::Surface(SDL_Surface* surface)
	: surface_(surface)
{

}

Surface::Surface(const Surface& other)
	: surface_(nullptr)
{
	if (other.surface_) {
		surface_ = SDL_CreateRGBSurface(
			0,
			other.Width(), other.Height(), 32,
			RGBAMasks::rmask,
			RGBAMasks::gmask,
			RGBAMasks::bmask,
			RGBAMasks::amask
		);

		if (SDL_BlitSurface(other.surface_, nullptr, surface_, nullptr) < 0) {
			std::cerr << "Surface::Surface(const Surface&): " << SDL_GetError()
				<< std::endl;
		}
	}
}

Surface::Surface(Surface&& other)
	: surface_(other.surface_)
{
	other.surface_ = nullptr;
}

Surface::~Surface() {
	SDL_FreeSurface(surface_);
}

Surface& Surface::operator=(const Surface& other) {
	if (this != &other && other.surface_) {
		SDL_FreeSurface(surface_);

		surface_ = SDL_CreateRGBSurface(
			0,
			other.Width(), other.Height(), 32,
			RGBAMasks::rmask,
			RGBAMasks::gmask,
			RGBAMasks::bmask,
			RGBAMasks::amask
		);

		if (SDL_BlitSurface(other.surface_, nullptr, surface_, nullptr) < 0) {
			std::cerr << "Surface::operator=(const Surface&): " << SDL_GetError()
				<< std::endl;
		}
	}
	return *this;
}

Surface& Surface::operator=(Surface&& other) {
	if (this != &other) {
		SDL_FreeSurface(surface_);
		surface_ = other.surface_;
		other.surface_ = nullptr;
	}
	return *this;
}
