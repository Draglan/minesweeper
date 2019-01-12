#ifndef BUTTON_H
#define BUTTON_H

#include "Drawable.h"
#include "Font.h"
#include <string>
#include <SDL.h>

typedef void(*ButtonCallback)();

class Button : public Drawable {
public:
	Button(const std::string& text, Font* f, int x, int y,
		ButtonCallback cb = nullptr)
		: text_(text), font_(f), x_(x), y_(y), callback_(cb)
	{
		if (font_) font_->SizeText(text, &w_, &h_);
	}

	Button(const Button&) = default;
	Button& operator=(const Button&) = default;

	int X() const {return x_;}
	int Y() const {return y_;}
	void SetPosition(int x, int y) {x_ = x; y_ = y;}

	int Width() const {return w_;}
	int Height() const {return h_;}

	const std::string& Text() const {return text_;}

	virtual void HandleInput(const SDL_Event& ev) override;
	virtual void Update(Uint32 ticks) override;
	virtual void Draw(const Window& w) const override;

private:
	bool IsInButton(int x, int y) {
		return x >= x_ && x < x_ + w_ && y >= y_ && y < y_ + h_;
	}

	void OnMouseEnter();
	void OnMouseLeave();
	void OnLeftClick();

	SDL_Color pressedBg_ = {150,150,150,0};

	SDL_Color bg_ = {255,255,255,0};
	SDL_Color fg_ = {255,255,255,255};

	const float fadeSpeed_ = 700.0f;
	const float maxAlpha_ = 150.0f;
	const float minAlpha_ = 0.0f;
	float alpha_ = 0.0f;
	bool isFading_ = false;

	std::string text_;
	Font* font_;

	int x_, y_, w_=0, h_=0;
	ButtonCallback callback_;

	bool pressed_ = false;
};

#endif /* BUTTON_H */
