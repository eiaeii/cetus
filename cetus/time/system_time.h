#pragma once

#include <cstdint>
#include <ctime>
namespace cetus
{
	struct SystemTime
	{
		//FixMe!
		static void LocalTime(int32_t& Year, int32_t& Month, int32_t& DayOfWeek, int32_t& Day, int32_t& Hour, int32_t& Min, int32_t& Sec, int32_t& MSec)
		{
			std::time_t t = std::time(nullptr);
			std::tm st = *std::localtime(&t);
			Year = st.tm_year;
			Month = st.tm_mon;
			DayOfWeek = st.tm_wday;
			Day = st.tm_mday;
			Hour = st.tm_hour;
			Min = st.tm_min;
			Sec = st.tm_sec;
			MSec = 0; //FixMe
		}
		static void UtcTime(int32_t& Year, int32_t& Month, int32_t& DayOfWeek, int32_t& Day, int32_t& Hour, int32_t& Min, int32_t& Sec, int32_t& MSec)
		{
			std::time_t t = std::time(nullptr);
			std::tm st = *std::gmtime(&t);
			Year = st.tm_year;
			Month = st.tm_mon;
			DayOfWeek = st.tm_wday;
			Day = st.tm_mday;
			Hour = st.tm_hour;
			Min = st.tm_min;
			Sec = st.tm_sec;
			MSec = 0; //FixMe
		}
	};
}