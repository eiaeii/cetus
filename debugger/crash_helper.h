#pragma once
#ifdef _WIN32
#include <TCHAR.h>  
#include <Windows.h>  
#include <DbgHelp.h>  

namespace terra
{
	extern LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);
}

#endif