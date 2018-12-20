#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <string>

class Surface;
class Window;

class Texture {
public:
	Texture(const Window& wnd, const std::string& file);
	Texture(const Window& wnd, const Surface&);
	Texture(const Texture&) = delete;
	Texture(Texture&&);
	~Texture() {SDL_DestroyTexture(texture_);}

	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&);

	unsigned Width() const {return width_;}
	unsigned Height() const {return height_;}

	void GetColorMod(Uint8& r, Uint8& g, Uint8& b) const;
	void SetColorMod(Uint8 r, Uint8 g, Uint8 b);

	void GetAlphaMod(Uint8& a) const;
	void SetColorMod(Uint8 a);

	SDL_Texture* SDLTexture() const {return texture_;}
	operator SDL_Texture*() const {return texture_;}

private:
	SDL_Texture* texture_;
	unsigned width_, height_;
};

#endif /* TEXTURE_H */
