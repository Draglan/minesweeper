#ifndef POPUP_MENU_H
#define POPUP_MENU_H

#include "Drawable.h"
#include "Button.h"
#include "Font.h"
#include <string>
#include <vector>

class PopupMenu : public Drawable {
public:
	PopupMenu() : header_(), buttons_() {}
	PopupMenu(const std::string& header);

	PopupMenu(const PopupMenu&) = default;
	PopupMenu& operator=(const PopupMenu&) = default;

	virtual ~PopupMenu() {}

	// Drawable overrides
	//
	virtual void HandleInput(const SDL_Event& ev) override;
	virtual void Update(Uint32 ticks) override;
	virtual void Draw(const Window& w) const override;

	// Add a button to the menu.
	void AddButton(const std::string& text, ButtonCallback cb = nullptr);

	// Set/get the menu's header text.
	const std::string& GetHeaderText() const {return header_;}
	void SetHeaderText(const std::string& t) {header_ = t;}

	// Get the menu's dimensions.
	int X() const {return x_;}
	int Y() const {return y_;}
	int Width() const {return w_;}
	int Height() const {return h_;}

private:
	void CalcDimensions(int hdHeight);

	const int btnPtSize_ = 24;
	int x_=0, y_=0, w_=0, h_=0;

	std::string header_;
	std::vector<Button> buttons_;
	Font* hdFont_ = nullptr;
};

#endif /* POPUP_MENU_H */
