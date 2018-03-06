#include "data_time.h"
#include "system_time.h"
#include "stringutil/stringutil.h"
using namespace terra;

/* DateTime constants
*****************************************************************************/

const int32_t DateTime::kDaysPerMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const int32_t DateTime::kDaysToMonth[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };


/* DateTime structors
*****************************************************************************/

DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond)
{
	Expects(Validate(year, month, day, hour, minute, second, millisecond));

	int32_t total_days = 0;

	if ((month > 2) && IsLeapYear(year))
	{
		++total_days;
	}

	year--;											// the current year is not a full year yet
	month--;										// the current month is not a full month yet

	total_days += year * 365;
	total_days += year / 4;							// leap year day every four years...
	total_days -= year / 100;						// ...except every 100 years...
	total_days += year / 400;						// ...but also every 400 years
	total_days += kDaysToMonth[month];				// days in this year up to last month
	total_days += day - 1;							// days in this month minus today

	ticks_ = total_days * ETimespan::TicksPerDay
		+ hour * ETimespan::TicksPerHour
		+ minute * ETimespan::TicksPerMinute
		+ second * ETimespan::TicksPerSecond
		+ millisecond * ETimespan::TicksPerMillisecond;
}

void DateTime::GetDate(int32_t& out_year, int32_t& out_month, int32_t& out_day) const
{
	// Based on FORTRAN code in:
	// Fliegel, H. F. and van Flandern, T. C.,
	// Communications of the ACM, Vol. 11, No. 10 (October 1968).

	int32_t i, j, k, l, n;
	l = static_cast<int32_t>(std::floor(GetJulianDay() + 0.5)) + 68569;
	n = 4 * l / 146097;
	l = l - (146097 * n + 3) / 4;
	i = 4000 * (l + 1) / 1461001;
	l = l - 1461 * i / 4 + 31;
	j = 80 * l / 2447;
	k = l - 2447 * j / 80;
	l = j / 11;
	j = j + 2 - 12 * l;
	i = 100 * (n - 49) + i + l;

	out_year = i;
	out_month = j;
	out_day = k;
}


int32_t DateTime::GetDay() const
{
	int32_t year, month, day;
	GetDate(year, month, day);

	return day;
}


EDayOfWeek DateTime::GetDayOfWeek() const
{
	// January 1, 0001 was a Monday
	return static_cast<EDayOfWeek>((ticks_ / ETimespan::TicksPerDay) % 7);
}


int32_t DateTime::GetDayOfYear() const
{
	int32_t year, month, day;
	GetDate(year, month, day);

	for (int32_t current_month = 1; current_month < month; ++current_month)
	{
		day += DaysInMonth(year, current_month);
	}

	return day;
}


int32_t DateTime::GetHour12() const
{
	int32_t hour = GetHour();

	if (hour < 1)
	{
		return 12;
	}

	if (hour > 12)
	{
		return (hour - 12);
	}

	return hour;
}


int32_t DateTime::GetMonth() const
{
	int32_t year, month, day;
	GetDate(year, month, day);

	return month;
}


int32_t DateTime::GetYear() const
{
	int32_t year, month, day;
	GetDate(year, month, day);

	return year;
}



//bool DateTime::Serialize(FArchive& Ar)
//{
//	Ar << *this;
//
//	return true;
//}


std::string DateTime::ToIso8601() const
{
	return ToString("%Y-%m-%dT%H:%M:%S.%sZ");
}

std::string DateTime::ToHttpDate() const
{
	std::string daystr;
	switch (GetDayOfWeek())
	{
	case EDayOfWeek::Monday:	daystr = "Mon";	break;
	case EDayOfWeek::Tuesday:	daystr = "Tue";	break;
	case EDayOfWeek::Wednesday:	daystr = "Wed";	break;
	case EDayOfWeek::Thursday:	daystr = "Thu";	break;
	case EDayOfWeek::Friday:	daystr = "Fri";	break;
	case EDayOfWeek::Saturday:	daystr = "Sat";	break;
	case EDayOfWeek::Sunday:	daystr = "Sun";	break;
	}

	std::string monthstr;
	switch (GetMonthOfYear())
	{
	case EMonthOfYear::January:		monthstr = "Jan";	break;
	case EMonthOfYear::February:	monthstr = "Feb";	break;
	case EMonthOfYear::March:		monthstr = "Mar";	break;
	case EMonthOfYear::April:		monthstr = "Apr";	break;
	case EMonthOfYear::May:			monthstr = "May";	break;
	case EMonthOfYear::June:		monthstr = "Jun";	break;
	case EMonthOfYear::July:		monthstr = "Jul";	break;
	case EMonthOfYear::August:		monthstr = "Aug";	break;
	case EMonthOfYear::September:	monthstr = "Sep";	break;
	case EMonthOfYear::October:		monthstr = "Oct";	break;
	case EMonthOfYear::November:	monthstr = "Nov";	break;
	case EMonthOfYear::December:	monthstr = "Dec";	break;
	}

	std::string time = StringUtils::Format("%02i:%02i:%02i", GetHour(), GetMinute(), GetSecond());
	return StringUtils::Format("%s, %02d %s %d %s GMT", daystr.c_str(), GetDay(), monthstr.c_str(), GetYear(), time.c_str());
}

std::string DateTime::ToString() const
{
	return ToString("%Y.%m.%d-%H.%M.%S");
}


std::string DateTime::ToString(const char* fmt) const
{
	std::string result;

	if (fmt != nullptr)
	{
		while (*fmt != '\0')
		{
			if ((*fmt == '%') && (*(++fmt) != '\0'))
			{
				switch (*fmt)
				{
				case 'a': result += IsMorning() ? "am" : "pm"; break;
				case 'A': result += IsMorning() ? "AM" : "PM"; break;
				case 'd': result += StringUtils::Format("%02i", GetDay()); break;
				case 'D': result += StringUtils::Format("%03i", GetDayOfYear()); break;
				case 'm': result += StringUtils::Format("%02i", GetMonth()); break;
				case 'y': result += StringUtils::Format("%02i", GetYear() % 100); break;
				case 'Y': result += StringUtils::Format("%04i", GetYear()); break;
				case 'h': result += StringUtils::Format("%02i", GetHour12()); break;
				case 'H': result += StringUtils::Format("%02i", GetHour()); break;
				case 'M': result += StringUtils::Format("%02i", GetMinute()); break;
				case 'S': result += StringUtils::Format("%02i", GetSecond()); break;
				case 's': result += StringUtils::Format("%03i", GetMillisecond()); break;
				default:		 result += *fmt;
				}
			}
			else
			{
				result += *fmt;
			}

			// move to the next one
			fmt++;
		}
	}

	return result;
}


/* DateTime static interface
*****************************************************************************/

int32_t DateTime::DaysInMonth(int32_t year, int32_t month)
{
	Expects((month >= 1) && (month <= 12));

	if ((month == 2) && IsLeapYear(year))
	{
		return 29;
	}

	return kDaysPerMonth[month];
}


int32_t DateTime::DaysInYear(int32_t year)
{
	if (IsLeapYear(year))
	{
		return 366;
	}

	return 365;
}


bool DateTime::IsLeapYear(int32_t year)
{
	if ((year % 4) == 0)
	{
		return (((year % 100) != 0) || ((year % 400) == 0));
	}

	return false;
}


DateTime DateTime::Now()
{
	int32_t year, month, day, day_of_week;
	int32_t hour, minute, second, millisecond;
	SystemTime::LocalTime(year, month, day_of_week, day, hour, minute, second, millisecond);

	return DateTime(year, month, day, hour, minute, second, millisecond);
}


bool DateTime::Parse(const std::string& datatime_string, DateTime& outDateTime)
{
	// first replace -, : and . with space
	std::string fixed_string = datatime_string;
	StringUtils::Replace(fixed_string, '-', ' ');
	StringUtils::Replace(fixed_string, ':', ' ');
	StringUtils::Replace(fixed_string, '.', ' ');
	std::vector<std::string> tokens;
	StringUtils::Split(fixed_string, " ", tokens);

	// make sure it parsed it properly (within reason)
	if ((tokens.size() < 6) || (tokens.size() > 7))
	{
		return false;
	}

	const int32_t year = atoi(tokens[0].c_str());
	const int32_t month = atoi(tokens[1].c_str());
	const int32_t day = atoi(tokens[2].c_str());
	const int32_t hour = atoi(tokens[3].c_str());
	const int32_t minute = atoi(tokens[4].c_str());
	const int32_t second = atoi(tokens[5].c_str());
	const int32_t millisecond = tokens.size() > 6 ? atoi(tokens[6].c_str()) : 0;

	if (!Validate(year, month, day, hour, minute, second, millisecond))
	{
		return false;
	}

	// convert the tokens to numbers
	outDateTime.ticks_ = DateTime(year, month, day, hour, minute, second, millisecond).ticks_;

	return true;
}


bool DateTime::ParseIso8601(const char* datatime_string, DateTime& out_datatime)
{
	// DateOnly: YYYY-MM-DD
	// DateTime: YYYY-mm-ddTHH:MM:SS(.ssss)(Z|+th:tm|-th:tm)

	const char* ptr = datatime_string;
	char* next = nullptr;

	int32_t year = 0, month = 0, day = 0;
	int32_t hour = 0, minute = 0, second = 0, millisecond = 0;
	int32_t tz_hour = 0, tz_minute = 0;

	// get date
	year = strtol(ptr, &next, 10);

	if ((next <= ptr) || (*next == '\0'))
	{
		return false;
	}

	ptr = next + 1; // skip separator
	month = strtol(ptr, &next, 10);

	if ((next <= ptr) || (*next == '\0'))
	{
		return false;
	}

	ptr = next + 1; // skip separator
	day = strtol(ptr, &next, 10);

	if (next <= ptr)
	{
		return false;
	}

	// check whether this is date and time
	if (*next == 'T')
	{
		ptr = next + 1;

		// parse time
		hour = strtol(ptr, &next, 10);

		if ((next <= ptr) || (*next == '\0'))
		{
			return false;
		}

		ptr = next + 1; // skip separator
		minute = strtol(ptr, &next, 10);

		if ((next <= ptr) || (*next == '\0'))
		{
			return false;
		}

		ptr = next + 1; // skip separator
		second = strtol(ptr, &next, 10);

		if (next <= ptr)
		{
			return false;
		}

		// check for milliseconds
		if (*next == '.')
		{
			ptr = next + 1;
			millisecond = strtol(ptr, &next, 10);

			// should be no more than 3 digits
			if ((next <= ptr) || (next > ptr + 3))
			{
				return false;
			}

			for (auto Digits = next - ptr; Digits < 3; ++Digits)
			{
				millisecond *= 10;
			}
		}

		// see if the timezone offset is included
		if (*next == '+' || *next == '-')
		{
			// include the separator since it's + or -
			ptr = next;

			// parse the timezone offset
			tz_hour = strtol(ptr, &next, 10);

			if ((next <= ptr) || (*next == '\0'))
			{
				return false;
			}

			ptr = next + 1; // skip separator
			tz_minute = strtol(ptr, &next, 10);

			if (next <= ptr)
			{
				return false;
			}
		}
		else if ((*next != '\0') && (*next != 'Z'))
		{
			return false;
		}
	}
	else if (*next != '\0')
	{
		return false;
	}

	if (!Validate(year, month, day, hour, minute, second, millisecond))
	{
		return false;
	}

	DateTime final_time(year, month, day, hour, minute, second, millisecond);

	// adjust for the timezone (bringing the DateTime into UTC)
	int32_t tz_offset_minutes = (tz_hour < 0) ? tz_hour * 60 - tz_minute : tz_hour * 60 + tz_minute;
	final_time -= Timespan(0, tz_offset_minutes, 0);
	out_datatime = final_time;

	return true;
}

/**
* Parse a string of the HTTP-date format from a web server
* https://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3.1
*
*   HTTP-date    = rfc1123-date | rfc850-date | asctime-date
*	 rfc1123-date = wkday "," SP date1 SP time SP "GMT"
*	 rfc850-date  = weekday "," SP date2 SP time SP "GMT"
*   asctime-date = wkday SP date3 SP time SP 4DIGIT
*   date1        = 2DIGIT SP month SP 4DIGIT
*				  ; day month year (e.g., 02 Jun 1982)
*   date2        = 2DIGIT "-" month "-" 2DIGIT
*				  ; day-month-year (e.g., 02-Jun-82)
*   date3        = month SP ( 2DIGIT | ( SP 1DIGIT ))
*				  ; month day (e.g., Jun  2)
*   time         = 2DIGIT ":" 2DIGIT ":" 2DIGIT
*				  ; 00:00:00 - 23:59:59
*   wkday        = "Mon" | "Tue" | "Wed"
*				  | "Thu" | "Fri" | "Sat" | "Sun"
*   weekday      = "Monday" | "Tuesday" | "Wednesday"
*				  | "Thursday" | "Friday" | "Saturday" | "Sunday"
*   month        = "Jan" | "Feb" | "Mar" | "Apr"
*				  | "May" | "Jun" | "Jul" | "Aug"
*				  | "Sep" | "Oct" | "Nov" | "Dec"
*/
bool DateTime::ParseHttpDate(const std::string& httpdate, DateTime& out_datetime)
{
	auto ParseTime = [](const std::string& time_string, int32_t& hour, int32_t& minute, int32_t& second) -> bool
	{
		// 2DIGIT ":" 2DIGIT ":" 2DIGIT
		// ; 00:00 : 00 - 23 : 59 : 59
		std::vector<std::string> tokens;
		// split up on a single delimiter
		int32_t num_tokens = StringUtils::Split(time_string, ":", tokens);
		if (num_tokens == 3)
		{
			hour = atoi(tokens[0].c_str());
			minute = atoi(tokens[1].c_str());
			second = atoi(tokens[2].c_str());
			return (hour >= 0 && hour < 24) && (minute >= 0 && minute <= 59) && (second >= 0 && second <= 59);
		}

		return false;
	};

	auto parse_wkday = [](const std::string& wkday) -> int32_t
	{
		const auto num_chars = wkday.length();
		if (num_chars == 3)
		{
			if (StringUtils::IsEqual(wkday.c_str(), "Mon"))
			{
				return 1;
			}
			else if (StringUtils::IsEqual(wkday.c_str(), "Tue"))
			{
				return 2;
			}
			else if (StringUtils::IsEqual(wkday.c_str(), "Wed"))
			{
				return 3;
			}
			else if (StringUtils::IsEqual(wkday.c_str(), "Thu"))
			{
				return 4;
			}
			else if (StringUtils::IsEqual(wkday.c_str(), "Fri"))
			{
				return 5;
			}
			else if (StringUtils::IsEqual(wkday.c_str(), "Sat"))
			{
				return 6;
			}
			else if (StringUtils::IsEqual(wkday.c_str(), "Sun"))
			{
				return 7;
			}
		}

		return -1;
	};

	auto ParseWeekday = [](const std::string& weekday) -> int32_t
	{
		const auto num_chars = weekday.length();
		if (num_chars >= 6 && num_chars <= 9)
		{
			if (StringUtils::IsEqual(weekday.c_str(), "Monday"))
			{
				return 1;
			}
			else if (StringUtils::IsEqual(weekday.c_str(), "Tueday"))
			{
				return 2;
			}
			else if (StringUtils::IsEqual(weekday.c_str(), "Wednesday"))
			{
				return 3;
			}
			else if (StringUtils::IsEqual(weekday.c_str(), "Thursday"))
			{
				return 4;
			}
			else if (StringUtils::IsEqual(weekday.c_str(), "Friday"))
			{
				return 5;
			}
			else if (StringUtils::IsEqual(weekday.c_str(), "Saturday"))
			{
				return 6;
			}
			else if (StringUtils::IsEqual(weekday.c_str(), "Sunday"))
			{
				return 7;
			}
		}

		return -1;
	};

	auto parse_month = [](const std::string& month) -> int32_t
	{
		const auto num_chars = month.length();
		if (num_chars == 3)
		{
			if (StringUtils::IsEqual(month.c_str(), "Jan"))
			{
				return 1;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Feb"))
			{
				return 2;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Mar"))
			{
				return 3;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Apr"))
			{
				return 4;
			}
			else if (StringUtils::IsEqual(month.c_str(), "May"))
			{
				return 5;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Jun"))
			{
				return 6;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Jul"))
			{
				return 7;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Aug"))
			{
				return 8;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Sep"))
			{
				return 9;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Oct"))
			{
				return 10;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Nov"))
			{
				return 11;
			}
			else if (StringUtils::IsEqual(month.c_str(), "Dec"))
			{
				return 12;
			}
		}

		return -1;
	};

	auto parse_date1 = [parse_month](const std::string& daystr, const std::string& monstr, const std::string& yearstr, int32_t& month, int32_t& day, int32_t& year) -> bool
	{
		// date1 = 2DIGIT SP month SP 4DIGIT
		// ; day month year(e.g., 02 Jun 1982)

		day = atoi(daystr.c_str());
		month = parse_month(monstr);
		year = (yearstr.length() == 4) ? atoi(yearstr.c_str()) : -1;

		return (day > 0 && day <= 31) && (month > 0 && month <= 12) && (year > 0 && year <= 9999);
	};

	auto parse_date2 = [parse_month](const std::string& date2, int32_t& month, int32_t& day, int32_t& year) -> bool
	{
		// date2 = 2DIGIT "-" month "-" 2DIGIT
		// ; day - month - year(e.g., 02 - Jun - 82)
		std::vector<std::string> tokens;

		// split up on a single delimiter
		int32_t num_tokens = StringUtils::Split(date2, "-", tokens);
		if (num_tokens == 3)
		{
			day = atoi(tokens[0].c_str());
			month = parse_month(tokens[1]);
			year = atoi(tokens[2].c_str());

			// Horrible assumption here, but this is a deprecated part of the spec
			year += 1900;
		}

		return (day > 0 && day <= 31) && (month > 0 && month <= 12) && (year > 0 && year <= 9999);
	};

	auto parse_date3 = [parse_month](const std::string& monstr, const std::string& daystr, int32_t& month, int32_t& day) -> bool
	{
		// date3 = month SP(2DIGIT | (SP 1DIGIT))
		// ; month day(e.g., Jun  2)
		const auto num_digits = daystr.length();
		day = (num_digits > 0 && num_digits <= 2) ? atoi(daystr.c_str()) : -1;
		month = parse_month(monstr);

		return (day > 0 && day <= 31) && (month > 0 && month <= 12);
	};

	if (!httpdate.empty())
	{
		std::vector<std::string> tokens;

		// split up on a single delimiter
		int32_t num_tokens = StringUtils::Split(httpdate, "-", tokens);
		if (num_tokens > 0)
		{
			int32_t month = 0;
			int32_t day = 0;
			int32_t year = 0;
			int32_t hour = 0;
			int32_t minute = 0;
			int32_t second = 0;
			int32_t millisecond = 0;

			if (StringUtils::EndsWith(tokens[0], ","))
			{
				tokens[0].erase(tokens[0].length() - 1, 1);
			}

			if (StringUtils::IsEqual(tokens[tokens.size() - 1].c_str(), "GMT"))
			{
				// rfc1123 - date | rfc850 - date 
				if (tokens.size() == 6)
				{
					int32_t wkday = parse_wkday(tokens[0]);
					if (wkday > 0)
					{
						// rfc1123 - date = wkday "," SP date1 SP time SP "GMT"
						if (parse_date1(tokens[1], tokens[2], tokens[3], month, day, year))
						{
							ParseTime(tokens[4], hour, minute, second);
						}
					}
				}
				else if (tokens.size() == 4)
				{
					// rfc850 - date = weekday "," SP date2 SP time SP "GMT"
					int32_t WeekDay = ParseWeekday(tokens[0]);
					if (WeekDay > 0)
					{
						if (parse_date2(tokens[1], month, day, year))
						{
							ParseTime(tokens[2], hour, minute, second);
						}
					}
				}

			}
			else if (tokens.size() == 5)
			{
				// asctime - date = wkday SP date3 SP time SP 4DIGIT
				int32_t WkDay = parse_wkday(tokens[0]);
				if (WkDay > 0)
				{
					if (parse_date3(tokens[1], tokens[2], month, day))
					{
						if (ParseTime(tokens[3], hour, minute, second))
						{
							if (tokens[4].length() == 4)
							{
								year = atoi(tokens[4].c_str());
							}
						}
					}
				}
			}

			if (Validate(year, month, day, hour, minute, second, millisecond))
			{
				// convert the tokens to numbers
				out_datetime = DateTime(year, month, day, hour, minute, second, millisecond);
				return true;
			}
		}
	}

	return false;
}

DateTime DateTime::UtcNow()
{
	int32_t year, month, day, day_of_week;
	int32_t hour, minute, second, millisecond;

	SystemTime::UtcTime(year, month, day_of_week, day, hour, minute, second, millisecond);

	return DateTime(year, month, day, hour, minute, second, millisecond);
}


bool DateTime::Validate(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond)
{
	return (year >= 1) && (year <= 9999) &&
		(month >= 1) && (month <= 12) &&
		(day >= 1) && (day <= DaysInMonth(year, month)) &&
		(hour >= 0) && (hour <= 23) &&
		(minute >= 0) && (minute <= 59) &&
		(second >= 0) && (second <= 59) &&
		(millisecond >= 0) && (millisecond <= 999);
}


/* DateTime friend functions
*****************************************************************************/

//FArchive& operator<<(FArchive& Ar, DateTime& DateTime)
//{
//	return Ar << DateTime.ticks_;
//}

size_t terra::GetTypeHash(const DateTime & DateTime)
{
	int64_t val = DateTime.ticks_;
	return std::hash<int64_t>()(val);
}
