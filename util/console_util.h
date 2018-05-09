#pragma once

#include <cstdint>

namespace terra
{
	enum class EConsoleColor
	{
		DEFAULT,
		WHITE,
		GREEN,
		YELLOW,
		BLUE,
		CYAN,
		PINK,
		RED,
	};
	class ConsoleUtils
	{
	public:
		static void SetColor(EConsoleColor color);
		static void SetBufferSize(int16_t x, int16_t y);


	private:
		static void SetDefault();
		static void SetWhite();
		static void SetGreen();
		static void SetYellow();
		static void SetBlue();
		static void SetCyan();
		static void SetPink();
		static void SetRed();



		static void SetConsoleColor(uint16_t wAttributes);
	};
}