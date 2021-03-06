#pragma once

#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
namespace terra
{
	struct SystemTime
	{
		//FixMe!
		static void LocalTime(int32_t& Year, int32_t& Month, int32_t& DayOfWeek, int32_t& Day, int32_t& Hour, int32_t& Min, int32_t& Sec, int32_t& MSec)
		{
#ifdef _WIN32
			SYSTEMTIME st;
			GetLocalTime(&st);

			Year = st.wYear;
			Month = st.wMonth;
			DayOfWeek = st.wDayOfWeek;
			Day = st.wDay;
			Hour = st.wHour;
			Min = st.wMinute;
			Sec = st.wSecond;
			MSec = st.wMilliseconds;
#else
			// query for calendar time
			struct timeval Time;
			gettimeofday(&Time, NULL);

			// convert it to local time
			struct tm LocalTime;
			localtime_r(&Time.tv_sec, &LocalTime);

			// pull out data/time
			Year = LocalTime.tm_year + 1900;
			Month = LocalTime.tm_mon + 1;
			DayOfWeek = LocalTime.tm_wday;
			Day = LocalTime.tm_mday;
			Hour = LocalTime.tm_hour;
			Min = LocalTime.tm_min;
			Sec = LocalTime.tm_sec;
			MSec = Time.tv_usec / 1000;
#endif
		}
		static void UtcTime(int32_t& Year, int32_t& Month, int32_t& DayOfWeek, int32_t& Day, int32_t& Hour, int32_t& Min, int32_t& Sec, int32_t& MSec)
		{
#if defined(_WIN32)
			SYSTEMTIME st;
			GetSystemTime(&st);

			Year = st.wYear;
			Month = st.wMonth;
			DayOfWeek = st.wDayOfWeek;
			Day = st.wDay;
			Hour = st.wHour;
			Min = st.wMinute;
			Sec = st.wSecond;
			MSec = st.wMilliseconds;
#else
			// query for calendar time
			struct timeval Time;
			gettimeofday(&Time, NULL);

			// convert it to UTC
			struct tm LocalTime;
			gmtime_r(&Time.tv_sec, &LocalTime);

			// pull out data/time
			Year = LocalTime.tm_year + 1900;
			Month = LocalTime.tm_mon + 1;
			DayOfWeek = LocalTime.tm_wday;
			Day = LocalTime.tm_mday;
			Hour = LocalTime.tm_hour;
			Min = LocalTime.tm_min;
			Sec = LocalTime.tm_sec;
			MSec = Time.tv_usec / 1000;
#endif
		}
	};
}