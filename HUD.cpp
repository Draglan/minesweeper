#include "HUD.h"
#include "TextureFactory.h"
#include "ScreenWriter.h"
#include "Font.h"
#include "Window.h"
#include "MinesweeperState.h"
#include <sstream>
#include <iomanip>

HUD::HUD(MinesweeperState& s)
	: state_(s),
	mine_(*TextureFactory::Inst().GetTexture("mine.png")),
	clock_(*TextureFactory::Inst().GetTexture("clock.png")),
	doTimer_(false),
	time_(0), timer_(0)
{
}

void HUD::Update(Uint32 ticks) {
	if (doTimer_) {
		timer_ += ticks;
		if (timer_ >= 1000) {
			timer_ = 0;
			++time_;
		}
	}
}

void HUD::Draw(const Window& w) const {

	Font* f = ScreenWriter::Inst().GetFont("arial.ttf", 32);
	if (!f) return;

	// Draw bar across top of screen
	Rectangle r(0, 0, w.Width(), f->PtSize());
	w.DrawFilledRect(&r, {0,0,0,150});

	// Draw the timer and its icon
	DrawTimer(f, w);
	DrawMineCount(f, w);
}

void HUD::DrawTimer(Font* f, const Window& w) const {
	Uint32 mins = time_ / 60;
	Uint32 secs = time_ % 60;
	Rectangle r;

	// Draw timer
	//
	ScreenWriter& sw = ScreenWriter::Inst();
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << mins << ':'
		<< std::setw(2) << std::setfill('0') << secs;

	int timerW;
	f->SizeText(ss.str(), &timerW, nullptr);

	// draw the clock icon
	r = {
		static_cast<int>(w.Width()/2 - timerW/2 - clock_.Width()/2 - timerW/2 - 4),
		static_cast<int>(f->PtSize()/2-clock_.Height()/2),
		static_cast<int>(clock_.Width()),
		static_cast<int>(clock_.Height())
	};
	w.Draw(clock_, nullptr, &r);

	// draw the clock text
	sw.Write(f, {255,255,255,255}, r.x+f->PtSize(), 0, ss.str());
}

void HUD::DrawMineCount(Font* f, const Window& w) const {
	Rectangle r;
	ScreenWriter& sw = ScreenWriter::Inst();

	std::stringstream ss;
	ss << state_.GetMineCount() - state_.GetFlagsUsed();
	int mw;
	f->SizeText(ss.str(), &mw, nullptr);

	r = {
		static_cast<int>(w.Width()/2 - mine_.Width()/2 + f->PtSize() + 4),
		static_cast<int>(f->PtSize()/2-mine_.Height()/2),
		static_cast<int>(mine_.Width()),
		static_cast<int>(mine_.Height())
	};
	w.Draw(mine_, nullptr, &r);

	// draw mine count
	sw.Write(f, {255,255,255,255}, r.x+f->PtSize(), 0, ss.str(), true);
}
