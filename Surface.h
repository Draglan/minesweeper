#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>
#include <string>

namespace RGBAMasks {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	const Uint32 rmask = 0xff000000;
	const Uint32 gmask = 0x00ff0000;
	const Uint32 bmask = 0x0000ff00;
	const Uint32 amask = 0x000000ff;
#else
	const Uint32 rmask = 0x000000ff;
	const Uint32 gmask = 0x0000ff00;
	const Uint32 bmask = 0x00ff0000;
	const Uint32 amask = 0xff000000;
#endif
}

class Surface {
public:
	Surface(unsigned w, unsigned h);
	explicit Surface(const std::string file);
	Surface(const Surface&);
	Surface(SDL_Surface* surface);
	Surface(Surface&&);
	~Surface();

	Surface& operator=(const Surface&);
	Surface& operator=(Surface&&);

	unsigned Width() const {return surface_ ? surface_->w : 0;}
	unsigned Height() const {return surface_ ? surface_->h : 0;}

	SDL_Surface* SDLSurface() const {return surface_;}

	operator SDL_Surface*() const {return surface_;}

private:
	SDL_Surface* surface_;
};

#endif /* SURFACE_H */
