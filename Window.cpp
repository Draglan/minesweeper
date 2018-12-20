#include "Window.h"
#include "SDL_image.h"
#include <SDL_ttf.h>
#include <stdexcept>

Window::Window(const std::string& title, unsigned w, unsigned h)
	: window_(nullptr), renderer_(nullptr), width_(w), height_(h)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw std::runtime_error("Window::Window(): " + std::string(SDL_GetError()));
	}

	window_ = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h,
		SDL_WINDOW_SHOWN
	);

	if (!window_) {
		SDL_Quit();
		throw std::runtime_error("Window::Window(): " + std::string(SDL_GetError()));
	}

	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer_) {
		SDL_DestroyWindow(window_);
		SDL_Quit();
		throw std::runtime_error("Window::Window(): " + std::string(SDL_GetError()));
	}

	int flags = IMG_INIT_PNG;
	if (IMG_Init(flags) != flags) {
		SDL_DestroyWindow(window_);
		SDL_DestroyRenderer(renderer_);
		SDL_Quit();
		throw std::runtime_error("Window::Window(): " + std::string(IMG_GetError()));
	}

	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

Window::Window(Window&& other)
	: window_(other.window_), renderer_(other.renderer_),
	width_(other.width_), height_(other.height_)
{
	other.window_ = nullptr;
	other.renderer_ = nullptr;
}

Window::~Window() {
	IMG_Quit();
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

Window& Window::operator=(Window&& other) {
	if (this != &other) {
		width_ = other.width_;
		height_ = other.height_;

		window_ = other.window_;
		other.window_ = nullptr;

		renderer_ = other.renderer_;
		other.renderer_ = nullptr;
	}
	return *this;
}

void Window::ClearScreen(Uint8 r, Uint8 g, Uint8 b) const {
	Uint8 or_, og, ob, oa;
	SDL_GetRenderDrawColor(renderer_, &or_, &og, &ob, &oa);
	SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
	SDL_RenderClear(renderer_);
	SDL_SetRenderDrawColor(renderer_, or_, og, ob, oa);
}

void Window::Draw(const Texture& t, const Rectangle* src, const Rectangle* dst)
	const
{
	SDL_RenderCopy(renderer_, t,
		reinterpret_cast<const SDL_Rect*>(src),
		reinterpret_cast<const SDL_Rect*>(dst));
}

void Window::Draw(const Texture& t, const Rectangle* src, const Rectangle* dst,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	Uint8 or_, og, ob, oa;
	SDL_GetTextureColorMod(t, &or_, &og, &ob);
	SDL_GetTextureAlphaMod(t, &oa);
	SDL_SetTextureColorMod(t, r, g, b);
	SDL_SetTextureAlphaMod(t, a);
	Draw(t, src, dst);
	SDL_SetTextureColorMod(t, or_, og, ob);
	SDL_SetTextureAlphaMod(t, oa);
}

void Window::Draw(const Texture& t, const Rectangle* src, const Rectangle* dst,
	double angle, SDL_RendererFlip flip) const
{
	SDL_RenderCopyEx(
		renderer_, t,
		reinterpret_cast<const SDL_Rect*>(src),
		reinterpret_cast<const SDL_Rect*>(dst),
		angle, nullptr, flip
	);
}

void Window::Draw(
	const Texture& t,
	const Rectangle* src, const Rectangle* dst,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a,
	double angle, SDL_RendererFlip flip) const
{
	Uint8 or_, og, ob, oa;
	SDL_GetTextureColorMod(t, &or_, &og, &ob);
	SDL_GetTextureAlphaMod(t, &oa);
	SDL_SetTextureColorMod(t, r, g, b);
	SDL_SetTextureAlphaMod(t, a);

	SDL_RenderCopyEx(
		renderer_, t,
		reinterpret_cast<const SDL_Rect*>(src),
		reinterpret_cast<const SDL_Rect*>(dst),
		angle, nullptr, flip
	);

	SDL_SetTextureColorMod(t, or_, og, ob);
	SDL_SetTextureAlphaMod(t, oa);
}

void Window::Draw(const Texture& t, int x, int y) const {
	SDL_Rect dst = {
		x, y,
		static_cast<int>(t.Width()),
		static_cast<int>(t.Height())
	};

	SDL_RenderCopy(renderer_, t, nullptr, &dst);
}

void Window::DrawFilledRect(const Rectangle* rect, SDL_Color col) const {
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer_, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer_, col.r, col.g, col.b, col.a);
	SDL_RenderFillRect(renderer_, reinterpret_cast<const SDL_Rect*>(rect));
	SDL_SetRenderDrawColor(renderer_, old.r, old.g, old.b, old.a);
}

void Window::DrawRect(const Rectangle* rect, SDL_Color col) const {
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer_, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer_, col.r, col.g, col.b, col.a);
	SDL_RenderDrawRect(renderer_, reinterpret_cast<const SDL_Rect*>(rect));
	SDL_SetRenderDrawColor(renderer_, old.r, old.g, old.b, old.a);
}

void Window::DrawPoint(int x, int y, SDL_Color col) const {
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer_, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer_, col.r, col.g, col.b, col.a);
	SDL_RenderDrawPoint(renderer_, x, y);
	SDL_SetRenderDrawColor(renderer_, old.r, old.g, old.b, old.a);
}

void Window::DrawLine(int x1, int y1, int x2, int y2, SDL_Color col) const {
	SDL_Color old;
	SDL_GetRenderDrawColor(renderer_, &old.r, &old.g, &old.b, &old.a);
	SDL_SetRenderDrawColor(renderer_, col.r, col.g, col.b, col.a);
	SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
	SDL_SetRenderDrawColor(renderer_, old.r, old.g, old.b, old.a);
}
