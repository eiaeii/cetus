#include "console_util.h"
#include "logger/g3log_fwd.h"
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace terra;

void ConsoleUtils::SetColor(EConsoleColor color)
{
	switch (color)
	{
	case EConsoleColor::DEFAULT: SetDefault(); break;
	case EConsoleColor::WHITE: SetWhite(); break;
	case EConsoleColor::GREEN: SetGreen(); break;
	case EConsoleColor::YELLOW: SetYellow(); break;
	case EConsoleColor::BLUE: SetBlue(); break;
	case EConsoleColor::CYAN: SetCyan(); break;
	case EConsoleColor::PINK: SetPink(); break;
	case EConsoleColor::RED: SetRed(); break;
	default: break;
	}
}

void ConsoleUtils::SetBufferSize(int16_t x, int16_t y)
{
#ifdef _WIN32
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);	// 获得缓冲区句柄

	COORD coord;
	coord.X = x;
	coord.Y = y;

	if (!SetConsoleScreenBufferSize(hCon, coord))
	{
		LOGF(WARNING, "SetConsoleScreenBufferSize (%d)", GetLastError());
	}
#endif
}

void ConsoleUtils::SetDefault()
{
#ifdef _WIN32
	SetConsoleColor(0X0F);
#else
	std::cout << "\033[0m";
#endif
}

void ConsoleUtils::SetWhite()
{
#ifdef _WIN32
	SetConsoleColor(0X0F);
#else
	std::cout << "\033[37m";
#endif
}
void ConsoleUtils::SetGreen()
{
#ifdef _WIN32
	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
	std::cout << "\033[32m";
#endif
}
void ConsoleUtils::SetYellow()
{
#ifdef _WIN32
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
	std::cout << "\033[33m";
#endif
}
void ConsoleUtils::SetBlue()
{
#ifdef _WIN32
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
	std::cout << "\033[34m";
#endif
}
void ConsoleUtils::SetCyan()
{
#ifdef _WIN32
	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
	std::cout << "\033[36m";
#endif
}
void ConsoleUtils::SetPink()
{
#ifdef _WIN32
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
	std::cout << "\033[35m";
#endif
}
void ConsoleUtils::SetRed()
{
#ifdef _WIN32
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
	std::cout << "\033[31m";
#endif
}

void ConsoleUtils::SetConsoleColor(uint16_t wAttributes)
{
#ifdef _WIN32
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);	// 获得缓冲区句柄
	SetConsoleTextAttribute(hCon, wAttributes);		// 设置文本及背景颜色
#endif
}
