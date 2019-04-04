#include "PopupMenu.h"
#include "Game.h"
#include "Rectangle.h"
#include "ScreenWriter.h"

PopupMenu::PopupMenu(const std::string& header)
	: header_(header), buttons_()
{
	int hdH;
	hdFont_ = ScreenWriter::Inst().GetFont(
		Game::Inst().DefaultFontName(),
		Game::Inst().DefaultPtSize()
	);

	if (hdFont_) {
		hdFont_->SizeText(header, nullptr, &hdH);
		CalcDimensions(hdH);
	}
}

void PopupMenu::HandleInput(const SDL_Event& ev) {
	for (auto& b : buttons_)
		b.HandleInput(ev);
}

void PopupMenu::Update(Uint32 ticks) {
	for (auto& b : buttons_)
		b.Update(ticks);
}

void PopupMenu::Draw(const Window& w) const {
	// Draw background
	Rectangle r(x_, y_, w_, h_);
	w.DrawFilledRect(&r, {0, 0, 0, 150});

	// Draw header
	if (hdFont_) {
		int hdW;
		hdFont_->SizeText(header_, &hdW, nullptr);
		int x = Game::Inst().GetWindow().Width()/2 - hdW/2;
		ScreenWriter::Inst().Write(hdFont_, {255,255,255,255}, x, y_, header_, true);
	}

	// Draw the buttons
	for (auto& b : buttons_)
		b.Draw(w);
}

void PopupMenu::
AddButton(const std::string& text, ButtonCallback cb) {
	const std::string& fName = Game::Inst().DefaultFontName();

	// Get the fonts
	Font* f = ScreenWriter::Inst().GetFont(fName, btnPtSize_);
	if (!f || !hdFont_) return;

	// Create the button
	buttons_.emplace_back(Button(text, f, 0, 0, cb));

	// Calculate new dimensions
	int hdH;
	hdFont_->SizeText(header_, nullptr, &hdH);

	CalcDimensions(hdH);

	// Re-position all of the buttons accordingly
	int ww = Game::Inst().GetWindow().Width();

	for (std::size_t i = 0; i < buttons_.size(); ++i) {
		Button& b = buttons_.at(i);
		b.SetPosition(
			ww/2 - b.Width()/2,
			y_ + hdH + b.Height() * i
		);
	}
}

void PopupMenu::CalcDimensions(int hdHeight) {
	x_ = 0;
	w_ = Game::Inst().GetWindow().Width();

	h_ = hdHeight;
	for (auto& b : buttons_) h_ += b.Height();

	y_ = Game::Inst().GetWindow().Height()/2 - h_/2;
}
