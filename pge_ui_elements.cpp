#include "pge_ui_elements.h"

// MENU:
pge_ui::Menu::Menu()
{
	reset();
}

void pge_ui::Menu::reset()
{
	vi2dWindowPosition = {0, 0};
	vi2dSize = {16, 8};
	i32tFontSize = 1;
	sztCursorPosition = 0;
	sztOptionCount = 0;
	vsOptions.clear();
}

void pge_ui::Menu::set_window_position(olc::vi2d vi2dNewWindowPosition)
{
	vi2dWindowPosition = vi2dNewWindowPosition;
}

void pge_ui::Menu::add_option(std::string sOption)
{
	vsOptions.push_back(sOption);
	++sztOptionCount;
	if(sOption.size() + 16 * i32tFontSize > vi2dSize.x)
		vi2dSize.x = sOption.size() + 16 * i32tFontSize;
	vi2dSize.y += 8 * i32tFontSize;
}

void pge_ui::Menu::show(olc::PixelGameEngine *pge)
{
	for (int32_t i = 0; i < sztOptionCount; i++)
	{
		pge->DrawString(vi2dWindowPosition + olc::vi2d{1, 1 + 8 * i32tFontSize*i},
							(i == sztCursorPosition ? ">>" : "  ") + vsOptions[i],
							olc::WHITE,
							i32tFontSize);
	}
}

// Returns name of selected option
std::string pge_ui::Menu::check_buttons(olc::PixelGameEngine *pge)
{
	// Input with arrow keys:
	if(pge->GetKey(olc::Key::DOWN).bPressed)
	{
		if (sztCursorPosition + 1 == sztOptionCount)
			sztCursorPosition = 0;
		else
			++sztCursorPosition;
	}
	if(pge->GetKey(olc::Key::UP).bPressed)
	{
		if (sztCursorPosition == 0)
			sztCursorPosition = sztOptionCount - 1;
		else
			--sztCursorPosition;
	}
	// Input with miscellaneous keys:
	if(pge->GetKey(olc::Key::ENTER).bPressed)
		return vsOptions[sztCursorPosition];
	return "";
}

// LABEL
pge_ui::Label::Label()
{
	reset();
}

void pge_ui::Label::reset()
{
	sName = "";
	vi2dWindowPosition = {0, 0};
	vi2dSize = {0, 0};
	i32tFontSize = 1;
	pxlFontColor = olc::WHITE;
}

void pge_ui::Label::set_name(std::string sNewName)
{
	sName = sNewName;
	vi2dSize.x = sNewName.size() * i32tFontSize;
}

void pge_ui::Label::set_window_position(olc::vi2d vi2dNewWindowPosition)
{
	vi2dWindowPosition = vi2dNewWindowPosition;
}

void pge_ui::Label::set_font_size(int32_t i32tNewFontSize)
{
	vi2dSize /= i32tFontSize;
	i32tFontSize = i32tNewFontSize;
	vi2dSize *= i32tFontSize;
}

void pge_ui::Label::set_font_color(olc::Pixel pxlNewFontColor)
{
	pxlFontColor = pxlNewFontColor;
}

void pge_ui::Label::show(olc::PixelGameEngine *pge)
{
	pge->DrawString(vi2dWindowPosition, sName, pxlFontColor, i32tFontSize);
}

// FORM
pge_ui::Form::Form()
{
	reset();
}

void pge_ui::Form::reset()
{
	sFieldName = "";
	vi2dWindowPosition = {0, 0};
	vi2dSize = {0, 0};
	i32tFontSize = 1;
	pxlFontColor = olc::WHITE;
	sztCaretPosition = 0;
}

void pge_ui::Form::clear_field()
{
	sFieldName.clear();
	vi2dSize = {0, 0};
	sztCaretPosition = 0;
}

std::string pge_ui::Form::get_field()
{
	return sFieldName;
}

void pge_ui::Form::set_window_position(olc::vi2d vi2dNewWindowPosition)
{
	vi2dWindowPosition = vi2dNewWindowPosition;
}

void pge_ui::Form::set_font_size(int32_t i32tNewFontSize)
{
	vi2dSize /= i32tFontSize;
	i32tFontSize = i32tNewFontSize;
	vi2dSize *= i32tFontSize;
}

void pge_ui::Form::set_font_color(olc::Pixel pxlNewFontColor)
{
	pxlFontColor = pxlNewFontColor;
}

std::string pge_ui::Form::check_buttons(olc::PixelGameEngine *pge)
{
	// Input with arrow keys:
	if(pge->GetKey(olc::Key::LEFT).bPressed) 
	{
		if(sztCaretPosition)
			sztCaretPosition--;
	}
	if(pge->GetKey(olc::Key::RIGHT).bPressed)
	{
		if(sztCaretPosition < sFieldName.size())
			sztCaretPosition++;
	}
	// Input with standart alphanumeric keys:
	if(pge->GetKey(olc::Key::A).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'A');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'a');
	}
	if(pge->GetKey(olc::Key::B).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'B');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'b');
	}
	if(pge->GetKey(olc::Key::C).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'C');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'c');
	}
	if(pge->GetKey(olc::Key::D).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'D');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'd');
	}
	if(pge->GetKey(olc::Key::E).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'E');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'e');
	}
	if(pge->GetKey(olc::Key::F).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'F');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'f');
	}
	if(pge->GetKey(olc::Key::G).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'G');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'g');
	}
	if(pge->GetKey(olc::Key::H).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'H');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'h');
	}
	if(pge->GetKey(olc::Key::I).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'I');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'i');
	}
	if(pge->GetKey(olc::Key::J).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'J');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'j');
	}
	if(pge->GetKey(olc::Key::K).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'K');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'k');
	}
	if(pge->GetKey(olc::Key::L).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'L');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'l');
	}
	if(pge->GetKey(olc::Key::M).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'M');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'm');
	}
	if(pge->GetKey(olc::Key::N).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'N');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'n');
	}
	if(pge->GetKey(olc::Key::O).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'O');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'o');
	}
	if(pge->GetKey(olc::Key::P).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'P');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'p');
	}
	if(pge->GetKey(olc::Key::Q).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'Q');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'q');
	}
	if(pge->GetKey(olc::Key::R).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'R');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'r');
	}
	if(pge->GetKey(olc::Key::S).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'S');
		else
			sFieldName.insert(sztCaretPosition++, 1, 's');
	}
	if(pge->GetKey(olc::Key::T).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'T');
		else
			sFieldName.insert(sztCaretPosition++, 1, 't');
	}
	if(pge->GetKey(olc::Key::U).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'U');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'u');
	}
	if(pge->GetKey(olc::Key::V).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'V');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'v');
	}
	if(pge->GetKey(olc::Key::W).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'W');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'w');
	}
	if(pge->GetKey(olc::Key::X).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'X');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'x');
	}
	if(pge->GetKey(olc::Key::Y).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'Y');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'y');
	}
	if(pge->GetKey(olc::Key::Z).bPressed)
	{
		if(pge->GetKey(olc::Key::SHIFT).bHeld)
			sFieldName.insert(sztCaretPosition++, 1, 'Z');
		else
			sFieldName.insert(sztCaretPosition++, 1, 'z');
	}
	// Input with standart numeric keys:
	if(pge->GetKey(olc::Key::K0).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '0');
	if(pge->GetKey(olc::Key::K1).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '1');
	if(pge->GetKey(olc::Key::K2).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '2');
	if(pge->GetKey(olc::Key::K3).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '3');
	if(pge->GetKey(olc::Key::K4).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '4');
	if(pge->GetKey(olc::Key::K5).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '5');
	if(pge->GetKey(olc::Key::K6).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '6');
	if(pge->GetKey(olc::Key::K7).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '7');
	if(pge->GetKey(olc::Key::K8).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '8');
	if(pge->GetKey(olc::Key::K9).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, '9');
	// Input with miscellaneous keys:
	if(pge->GetKey(olc::Key::SPACE).bPressed)
		sFieldName.insert(sztCaretPosition++, 1, ' ');
	if(pge->GetKey(olc::Key::BACK).bPressed) 
	{
		if(!sFieldName.empty())
			sFieldName.erase(--sztCaretPosition, 1);
	}
	if(pge->GetKey(olc::Key::DEL).bPressed) 
	{
		if(!sFieldName.empty())
			sFieldName.erase(sztCaretPosition, 1);
	}
	if(pge->GetKey(olc::Key::ENTER).bPressed)
	{
		return sFieldName;
	}
	return "";
}

void pge_ui::Form::show(olc::PixelGameEngine *pge)
{
	sFieldName.insert(sztCaretPosition, 1, '_');
	pge->DrawString(vi2dWindowPosition, sFieldName, pxlFontColor, i32tFontSize);
	sFieldName.erase(sztCaretPosition, 1);
}

pge_ui::Button::Button()
{
	reset();
}

void pge_ui::Button::reset()
{
	sName = "";
	vi2dWindowPosition = {0, 0};
	vi2dSize = {4, 4};
	i32tFontSize = 1;
	pxlFontColor = olc::WHITE;
	pxlFocusFontColor = olc::YELLOW;
}

void pge_ui::Button::set_name(std::string sNewName)
{
	sName = sNewName;
	vi2dSize = {sName.size(), 1};
	vi2dSize *= 8 * i32tFontSize;
	vi2dSize += {4, 4};
}

void pge_ui::Button::set_window_position(olc::vi2d vi2dNewWindowPosition)
{
	vi2dWindowPosition = vi2dNewWindowPosition;
}

void pge_ui::Button::set_font_size(int32_t i32tNewFontSize)
{
	vi2dSize -= {4, 4};
	vi2dSize /= i32tFontSize;
	i32tFontSize = i32tNewFontSize;
	vi2dSize *= i32tFontSize;
	vi2dSize += {4, 4};
}

void pge_ui::Button::set_font_color(olc::Pixel pxlNewFontColor)
{
	pxlFontColor = pxlNewFontColor;
}

void pge_ui::Button::set_focus_font_color(olc::Pixel pxlNewFocusFontColor)
{
	pxlFocusFontColor = pxlNewFocusFontColor;
}

bool pge_ui::Button::in_focus(olc::PixelGameEngine *pge)
{
	olc::vi2d vi2dCursorPosition = olc::vi2d(pge->GetMouseX(), pge->GetMouseY());
	if(vi2dCursorPosition.x >= vi2dWindowPosition.x 
	&& vi2dCursorPosition.x < vi2dWindowPosition.x + vi2dSize.x
	&& vi2dCursorPosition.y >= vi2dWindowPosition.y
	&& vi2dCursorPosition.y < vi2dWindowPosition.y + vi2dSize.y)
		return true;
	else
		return false;
}

bool pge_ui::Button::is_pressed(olc::PixelGameEngine *pge)
{
	if(in_focus(pge))
		if(pge->GetMouse(0).bPressed)
			return true;
	return false;
}

void pge_ui::Button::show(olc::PixelGameEngine *pge)
{
	if(in_focus(pge))
	{
		pge->DrawString(vi2dWindowPosition + olc::vi2d(4, 4), sName,
										pxlFocusFontColor, i32tFontSize);
		pge->DrawRect(vi2dWindowPosition, vi2dSize, pxlFocusFontColor);
	}
	else
	{
		pge->DrawString(vi2dWindowPosition + olc::vi2d(4, 4), sName,
										pxlFontColor, i32tFontSize);
		pge->DrawRect(vi2dWindowPosition, vi2dSize, pxlFontColor);
	}
}
