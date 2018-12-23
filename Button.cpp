#include "Button.h"
#include "ScreenWriter.h"
#include "Rectangle.h"
#include "Window.h"

void Button::HandleInput(const SDL_Event& ev) {
	if (ev.type == SDL_MOUSEMOTION) {
		int x = ev.motion.x;
		int y = ev.motion.y;
		int oldx = ev.motion.x - ev.motion.xrel;
		int oldy = ev.motion.y - ev.motion.yrel;

		if (!IsInButton(oldx, oldy) && IsInButton(x, y)) {
			OnMouseEnter();
		}
		else if (IsInButton(oldx, oldy) && !IsInButton(x, y)) {
			OnMouseLeave();
		}
	}
	else if (ev.type == SDL_MOUSEBUTTONDOWN) {
		if (IsInButton(ev.button.x, ev.button.y)) {
			pressed_ = true;
		}
	}
	else if (ev.type == SDL_MOUSEBUTTONUP) {
		if (pressed_ && IsInButton(ev.button.x, ev.button.y)) {
			OnLeftClick();
		}

		pressed_ = false;
	}
}

void Button::Update(Uint32 ticks) {
	if (isFading_) {
		alpha_ += ticks * fadeSpeed_ * 0.001f;
		if (alpha_ > maxAlpha_) alpha_ = maxAlpha_;
	}
	else {
		alpha_ -= ticks * fadeSpeed_ * 0.001f;
		if (alpha_ < minAlpha_) alpha_ = minAlpha_;
	}
}

void Button::Draw(const Window& w) const {
	Rectangle r(x_, y_, w_, h_);
	SDL_Color bgcol = pressed_ ? pressedBg_ : bg_;

	bgcol.a = static_cast<Uint8>(alpha_);

	w.DrawFilledRect(&r, bgcol);
	ScreenWriter::Inst().Write(font_, fg_, x_, y_, text_, true);
}

void Button::OnMouseEnter() {
	isFading_ = true;
}

void Button::OnMouseLeave() {
	isFading_ = false;
}

void Button::OnLeftClick() {
	if (callback_) callback_();
}
