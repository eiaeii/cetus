#include "crash_helper.h"

#ifdef _WIN32
namespace terra
{
	int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
	{
		// 定义函数指针  
		typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
			HANDLE,
			DWORD,
			HANDLE,
			MINIDUMP_TYPE,
			PMINIDUMP_EXCEPTION_INFORMATION,
			PMINIDUMP_USER_STREAM_INFORMATION,
			PMINIDUMP_CALLBACK_INFORMATION
			);
		// 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数  
		MiniDumpWriteDumpT pfnMiniDumpWriteDump = nullptr;
		HMODULE hDbgHelp = LoadLibrary(_T("DbgHelp.dll"));
		if (nullptr == hDbgHelp)
		{
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

		if (nullptr == pfnMiniDumpWriteDump)
		{
			FreeLibrary(hDbgHelp);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		// 创建 dmp 文件件  
		TCHAR szFileName[MAX_PATH] = { 0 };
		TCHAR* szVersion = _T("TerraDump1.0");
		SYSTEMTIME stLocalTime;
		GetLocalTime(&stLocalTime);
		wsprintf(szFileName, "%s-%04d%02d%02d-%02d%02d%02d.dmp",
			szVersion, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
			stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
		HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
		if (INVALID_HANDLE_VALUE == hDumpFile)
		{
			FreeLibrary(hDbgHelp);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		// 写入 dmp 文件  
		MINIDUMP_EXCEPTION_INFORMATION expParam;
		expParam.ThreadId = GetCurrentThreadId();
		expParam.ExceptionPointers = pExceptionPointers;
		expParam.ClientPointers = FALSE;
		//mini dump
		//pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			//hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : nullptr), nullptr, nullptr);
		//full dump
		DWORD Flags = MiniDumpWithFullMemory |
			MiniDumpWithFullMemoryInfo |
			MiniDumpWithHandleData |
			MiniDumpWithUnloadedModules |
			MiniDumpWithThreadInfo;
		pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDumpFile, (MINIDUMP_TYPE)Flags, (pExceptionPointers ? &expParam : nullptr), nullptr, nullptr);
		// 释放文件  
		CloseHandle(hDumpFile);
		FreeLibrary(hDbgHelp);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
	{
		// 这里做一些异常的过滤或提示  
		if (IsDebuggerPresent())
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}
		return GenerateMiniDump(lpExceptionInfo);
	}
}

#endif