#ifndef PGE_UI_ELEMENTS_H
#define PGE_UI_ELEMENTS_H

#include "olcPixelGameEngine.h"

namespace pge_ui
{
	class Menu
	{
	private:
		olc::vi2d vi2dWindowPosition; // in screen pixels (spx)
		olc::vi2d vi2dSize; // in spx
		int32_t i32tFontSize; // "i32tFontSize" * 8 spx = "Actual font size" (spx)
		std::size_t sztCursorPosition;
		std::size_t sztOptionCount;
		std::vector<std::string> vsOptions;
	public:
		Menu();
		void reset();
		void set_window_position(olc::vi2d vi2dNewWindowPosition);
		void add_option(std::string sOption);
		void show(olc::PixelGameEngine *pge);
		std::string check_buttons(olc::PixelGameEngine *pge);
	};

	class Label
	{
	private:
		std::string sName;
		olc::vi2d vi2dWindowPosition;
		olc::vi2d vi2dSize;
		int32_t i32tFontSize;
		olc::Pixel pxlFontColor;
	public:
		Label();
		void reset();
		void set_name(std::string sNewName);
		void set_window_position(olc::vi2d vi2dNewWindowPosition);
		void set_font_size(int32_t i32tNewFontSize);
		void set_font_color(olc::Pixel pxlNewFontColor);
		void show(olc::PixelGameEngine *pge);
	};

	class Form
	{
	private:
		std::string sFieldName;
		olc::vi2d vi2dWindowPosition;
		olc::vi2d vi2dSize;
		int32_t i32tFontSize;
		olc::Pixel pxlFontColor;
		std::size_t sztCaretPosition;
	public:
		Form();
		void reset();
		void clear_field();
		std::string get_field();
		void set_window_position(olc::vi2d vi2dNewWindowPosition);
		void set_font_size(int32_t i32tNewFontSize);
		void set_font_color(olc::Pixel pxlNewFontColor);
		std::string check_buttons(olc::PixelGameEngine *pge);
		void show(olc::PixelGameEngine *pge);
	};

	class Button
	{
	private:
		std::string sName;
		olc::vi2d vi2dWindowPosition;
		olc::vi2d vi2dSize;
		int32_t i32tFontSize;
		olc::Pixel pxlFontColor;
		olc::Pixel pxlFocusFontColor;
	public:
		Button();
		void reset();
		void set_name(std::string sNewName);
		void set_window_position(olc::vi2d vi2dNewWindowPosition);
		void set_font_size(int32_t i32tNewFontSize);
		void set_font_color(olc::Pixel pxlNewFontColor);
		void set_focus_font_color(olc::Pixel pxlNewFocusFontColor);
		bool in_focus(olc::PixelGameEngine *pge);
		bool is_pressed(olc::PixelGameEngine *pge);
		void show(olc::PixelGameEngine *pge);
	};
}

#endif // PGE_UI_ELEMENTS_H