#pragma once

#include "cetus_define.h"
#include "timespan.h"

namespace cetus
{
	/**
	* Enumerates the days of the week in 7-day calendars.
	*/
	enum class EDayOfWeek
	{
		Monday = 0,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
		Sunday
	};


	/**
	* Enumerates the months of the year in 12-month calendars.
	*/
	enum class EMonthOfYear
	{
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};


	/**
	* Implements a date and time.
	*
	* Values of this type represent dates and times between Midnight 00:00:00, January 1, 0001 and
	* Midnight 23:59:59.9999999, December 31, 9999 in the Gregorian calendar. Internally, the time
	* values are stored in ticks of 0.1 microseconds (= 100 nanoseconds) since January 1, 0001.
	*
	* To retrieve the current local date and time, use the FDateTime.Now() method. To retrieve the
	* current UTC time, use the FDateTime.UtcNow() method instead.
	*
	* This class also provides methods to convert dates and times from and to string representations,
	* calculate the number of days in a given month and year, check for leap years and determine the
	* time of day, day of week and month of year of a given date and time.
	*
	* The companion struct Timespan is provided for enabling date and time based arithmetic, such as
	* calculating the difference between two dates or adding a certain amount of time to a given date.
	*
	* Ranges of dates and times can be represented by the FDateRange class.
	*
	* @see FDateRange
	* @see Timespan
	*/
	struct DateTime
	{

	protected:

		/** Holds the days per month in a non-leap year. */
		static const int32_t kDaysPerMonth[];

		/** Holds the cumulative days per month in a non-leap year. */
		static const int32_t kDaysToMonth[];

	private:

		/** Holds the ticks in 100 nanoseconds resolution since January 1, 0001 A.D. */
		int64_t ticks_;
	public:

		/** Default constructor (no initialization). */
		DateTime() { }

		/**
		* Creates and initializes a new instance with the specified number of ticks.
		*
		* @param Ticks The ticks representing the date and time.
		*/
		DateTime(int64_t ticks)
			: ticks_(ticks)
		{ }

		/**
		* Creates and initializes a new instance with the specified year, month, day, hour, minute, second and millisecond.
		*
		* @param Year The year.
		* @param Month The month.
		* @param Day The day.
		* @param Hour The hour (optional).
		* @param Minute The minute (optional).
		* @param Second The second (optional).
		* @param Millisecond The millisecond (optional).
		*/
		DateTime(int32_t year, int32_t month, int32_t day, int32_t hour = 0, int32_t minute = 0, int32_t second = 0, int32_t millisecond = 0);

	public:

		/**
		* Returns result of adding the given time span to this date.
		*
		* @return A date whose value is the sum of this date and the given time span.
		* @see Timespan
		*/
		DateTime operator+(const Timespan& other) const
		{
			return DateTime(ticks_ + other.GetTicks());
		}

		/**
		* Adds the given time span to this date.
		*
		* @return This date.
		* @see Timespan
		*/
		DateTime& operator+=(const Timespan& other)
		{
			ticks_ += other.GetTicks();

			return *this;
		}

		/**
		* Returns time span between this date and the given date.
		*
		* @return A time span whose value is the difference of this date and the given date.
		* @see Timespan
		*/
		Timespan operator-(const DateTime& other) const
		{
			return Timespan(ticks_ - other.ticks_);
		}

		/**
		* Returns result of subtracting the given time span from this date.
		*
		* @return A date whose value is the difference of this date and the given time span.
		* @see Timespan
		*/
		DateTime operator-(const Timespan& other) const
		{
			return DateTime(ticks_ - other.GetTicks());
		}

		/**
		* Subtracts the given time span from this date.
		*
		* @return This date.
		* @see Timespan
		*/
		DateTime& operator-=(const Timespan& other)
		{
			ticks_ -= other.GetTicks();

			return *this;
		}

		/**
		* Compares this date with the given date for equality.
		*
		* @param other The date to compare with.
		* @return true if the dates are equal, false otherwise.
		*/
		bool operator==(const DateTime& other) const
		{
			return (ticks_ == other.ticks_);
		}

		/**
		* Compares this date with the given date for inequality.
		*
		* @param other The date to compare with.
		* @return true if the dates are not equal, false otherwise.
		*/
		bool operator!=(const DateTime& other) const
		{
			return (ticks_ != other.ticks_);
		}

		/**
		* Checks whether this date is greater than the given date.
		*
		* @param other The date to compare with.
		* @return true if this date is greater, false otherwise.
		*/
		bool operator>(const DateTime& other) const
		{
			return (ticks_ > other.ticks_);
		}

		/**
		* Checks whether this date is greater than or equal to the date span.
		*
		* @param other The date to compare with.
		* @return true if this date is greater or equal, false otherwise.
		*/
		bool operator>=(const DateTime& other) const
		{
			return (ticks_ >= other.ticks_);
		}

		/**
		* Checks whether this date is less than the given date.
		*
		* @param other The date to compare with.
		* @return true if this date is less, false otherwise.
		*/
		bool operator<(const DateTime& other) const
		{
			return (ticks_ < other.ticks_);
		}

		/**
		* Checks whether this date is less than or equal to the given date.
		*
		* @param other The date to compare with.
		* @return true if this date is less or equal, false otherwise.
		*/
		bool operator<=(const DateTime& other) const
		{
			return (ticks_ <= other.ticks_);
		}

	public:
		/**
		* Gets the date part of this date.
		*
		* The time part is truncated and becomes 00:00:00.000.
		*
		* @return A FDateTime object containing the date.
		*/
		DateTime GetDate() const
		{
			return DateTime(ticks_ - (ticks_ % ETimespan::TicksPerDay));
		}

		/**
		* Gets the date components of this date.
		*
		* @param outYear Will contain the year.
		* @param outMonth Will contain the number of the month (1-12).
		* @param outDay Will contain the number of the day (1-31).
		*/
		void GetDate(int32_t& outYear, int32_t& outMonth, int32_t& outDay) const;

		/**
		* Gets this date's day part (1 to 31).
		*
		* @return Day of the month.
		* @see GetHour, GetHour12, GetMillisecond, GetMinute, GetMonth, GetSecond, GetYear
		*/
		int32_t GetDay() const;

		/**
		* Calculates this date's day of the week (Sunday - Saturday).
		*
		* @return The week day.
		* @see GetDayOfYear, GetMonthOfYear, GetTimeOfDay
		*/
		EDayOfWeek GetDayOfWeek() const;

		/**
		* Gets this date's day of the year.
		*
		* @return The day of year.
		* @see GetDayOfWeek, GetMonthOfYear, GetTimeOfDay
		*/
		int32_t GetDayOfYear() const;

		/**
		* Gets this date's hour part in 24-hour clock format (0 to 23).
		*
		* @return The hour.
		* @see GetDay, GetDayOfWeek, GetDayOfYear, GetHour12, GetMillisecond, GetMinute, GetMonth, GetSecond, GetYear
		*/
		int32_t GetHour() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerHour) % 24);
		}

		/**
		* Gets this date's hour part in 12-hour clock format (1 to 12).
		*
		* @return The hour in AM/PM format.
		* @see GetDay, GetHour, GetMillisecond, GetMinute, GetMonth, GetSecond, GetYear
		*/
		int32_t GetHour12() const;

		/**
		* Returns the Julian Day for this date.
		*
		* The Julian Day is the number of days since the inception of the Julian calendar at noon on
		* Monday, January 1, 4713 B.C.E. The minimum Julian Day that can be represented in FDateTime is
		* 1721425.5, which corresponds to Monday, January 1, 0001 in the Gregorian calendar.
		*
		* @return Julian Day.
		* @see FromJulianDay, GetModifiedJulianDay
		*/
		double GetJulianDay() const
		{
			return (double)(1721425.5 + ticks_ / ETimespan::TicksPerDay);
		}

		/**
		* Returns the Modified Julian day.
		*
		* The Modified Julian Day is calculated by subtracting 2400000.5, which corresponds to midnight UTC on
		* November 17, 1858 in the Gregorian calendar.
		*
		* @return Modified Julian Day
		* @see GetJulianDay
		*/
		double GetModifiedJulianDay() const
		{
			return (GetJulianDay() - 2400000.5);
		}

		/**
		* Gets this date's millisecond part (0 to 999).
		*
		* @return The millisecond.
		* @see GetDay, GetHour, GetHour12, GetMinute, GetMonth, GetSecond, GetYear
		*/
		int32_t GetMillisecond() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerMillisecond) % 1000);
		}

		/**
		* Gets this date's minute part (0 to 59).
		*
		* @return The minute.
		* @see GetDay, GetHour, GetHour12, GetMillisecond, GetMonth, GetSecond, GetYear
		*/
		int32_t GetMinute() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerMinute) % 60);
		}

		/**
		* Gets this date's the month part (1 to 12).
		*
		* @return The month.
		* @see GetDay, GetHour, GetHour12, GetMillisecond, GetMinute, GetSecond, GetYear
		*/
		int32_t GetMonth() const;

		/**
		* Gets the date's month of the year (January to December).
		*
		* @return Month of year.
		* @see GetDayOfWeek, GetDayOfYear, GetTimeOfDay
		*/
		EMonthOfYear GetMonthOfYear() const
		{
			return static_cast<EMonthOfYear>(GetMonth());
		}

		/**
		* Gets this date's second part.
		*
		* @return The second.
		* @see GetDay, GetHour, GetHour12, GetMillisecond, GetMinute, GetMonth, GetYear
		*/
		int32_t GetSecond() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerSecond) % 60);
		}

		/**
		* Gets this date's representation as number of ticks.
		*
		* @return Number of ticks since midnight, January 1, 0001.
		*/
		int64_t GetTicks() const
		{
			return ticks_;
		}

		/**
		* Gets the time elapsed since midnight of this date.
		*
		* @param Time of day since midnight.
		* @see GetDayOfWeek, GetDayOfYear, GetMonthOfYear
		*/
		Timespan GetTimeOfDay() const
		{
			return Timespan(ticks_ % ETimespan::TicksPerDay);
		}

		/**
		* Gets this date's year part.
		*
		* @return The year.
		* @see GetDay, GetHour, GetHour12, GetMillisecond, GetMinute, GetMonth, GetSecond
		*/
		int32_t GetYear() const;

		/**
		* Gets whether this date's time is in the afternoon.
		*
		* @param true if it is in the afternoon, false otherwise.
		* @see IsMorning
		*/
		bool IsAfternoon() const
		{
			return (GetHour() >= 12);
		}

		/**
		* Gets whether this date's time is in the morning.
		*
		* @param true if it is in the morning, false otherwise.
		* @see IsAfternoon
		*/
		bool IsMorning() const
		{
			return (GetHour() < 12);
		}

		/**
		* Serializes this date and time from or into the specified archive.
		*
		* @param Ar The archive to serialize from or into.
		* @return true on success, false otherwise.
		*/
		//bool Serialize(FArchive& Ar);

		/**
		* Returns the ISO-8601 string representation of the FDateTime.
		*
		* The resulting string assumes that the FDateTime is in UTC.
		*
		* @return String representation.
		* @see ParseIso8601, ToString
		*/
		std::string ToIso8601() const;

		/**
		* Returns the RFC 1123 string representation of the FDateTime.
		*
		* The resulting string assumes that the FDateTime is in UTC.
		*
		* @return String representation.
		* @see ParseHttpDate, ToString
		*/
		std::string ToHttpDate() const;

		/**
		* Returns the string representation of this date using a default format.
		*
		* The returned string has the following format:
		*		yyyy.mm.dd-hh.mm.ss
		*
		* @return String representation.
		* @see Parse, ToIso8601
		*/
		std::string ToString() const;

		/**
		* Returns the string representation of this date.
		*
		* @param Format The format of the returned string.
		* @return String representation.
		* @see Parse, ToIso8601
		*/
		std::string ToString(const char* Format) const;

		/**
		* Returns this date as the number of seconds since the Unix Epoch (January 1st of 1970).
		*
		* @return Time of day.
		* @see FromUnixTimestamp
		*/
		int64_t ToUnixTimestamp() const
		{
			return (ticks_ - DateTime(1970, 1, 1).ticks_) / ETimespan::TicksPerSecond;
		}

	public:

		/**
		* Gets the number of days in the year and month.
		*
		* @param Year The year.
		* @param Month The month.
		* @return The number of days
		* @see DaysInYear
		*/
		static int32_t DaysInMonth(int32_t Year, int32_t Month);

		/**
		* Gets the number of days in the given year.
		*
		* @param Year The year.
		* @return The number of days.
		* @see DaysInMonth
		*/
		static int32_t DaysInYear(int32_t Year);

		/**
		* Returns the proleptic Gregorian date for the given Julian Day.
		*
		* @param JulianDay The Julian Day.
		* @return Gregorian date and time.
		* @see GetJulianDay
		*/
		static DateTime FromJulianDay(double JulianDay)
		{
			return DateTime((int64_t)((JulianDay - 1721425.5) * ETimespan::TicksPerDay));
		}

		/**
		* Returns the date from Unix time (seconds from midnight 1970-01-01)
		*
		* @param UnixTime Unix time (seconds from midnight 1970-01-01)
		* @return Gregorian date and time.
		* @see ToUnixTimestamp
		*/
		static DateTime FromUnixTimestamp(int64_t UnixTime)
		{
			return DateTime(1970, 1, 1) + Timespan(UnixTime * ETimespan::TicksPerSecond);
		}

		/**
		* Checks whether the given year is a leap year.
		*
		* A leap year is a year containing one additional day in order to keep the calendar synchronized
		* with the astronomical year. All years divisible by 4, but not divisible by 100 - except if they
		* are also divisible by 400 - are leap years.
		*
		* @param Year The year to check.
		* @return true if the year is a leap year, false otherwise.
		*/
		static bool IsLeapYear(int32_t Year);

		/**
		* Returns the maximum date value.
		*
		* The maximum date value is December 31, 9999, 23:59:59.9999999.
		*
		* @see MinValue
		*/
		static DateTime MaxValue()
		{
			return DateTime(3652059 * ETimespan::TicksPerDay - 1);
		}

		/**
		* Returns the minimum date value.
		*
		* The minimum date value is January 1, 0001, 00:00:00.0.
		*
		* @see MaxValue
		*/
		static DateTime MinValue()
		{
			return DateTime(0);
		}

		/**
		* Gets the local date and time on this computer.
		*
		* This method takes into account the local computer's time zone and daylight saving
		* settings. For time zone independent time comparisons, and when comparing times
		* between different computers, please use UtcNow() instead.
		*
		* @return Current date and time.
		* @see Today, UtcNow
		*/
		static DateTime Now();

		/**
		* Converts a string to a date and time.
		*
		* Currently, the string must be in the format written by either FDateTime.ToString() or
		* FTimeStamp.TimestampToFString(). other formats are not supported at this time.
		*
		* @param datatime_string The string to convert.
		* @param outDateTime Will contain the parsed date and time.
		* @return true if the string was converted successfully, false otherwise.
		*/
		static bool Parse(const std::string& datatime_string, DateTime& out_datatime);

		/**
		* Parses a date string in ISO-8601 format.
		*
		* @param datatime_string The string to be parsed
		* @param outDateTime FDateTime object (in UTC) corresponding to the input string (which may have been in any timezone).
		* @return true if the string was converted successfully, false otherwise.
		* @see Parse, ToIso8601
		*/
		static bool ParseIso8601(const char* datatime_string, DateTime& out_datatime);

		/**
		* Parses a date string in HTTP-date format (rfc1123-date | rfc850-date | asctime-date)
		* https://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3.1
		*
		* @param HttpDate The string to be parsed
		* @param outDateTime FDateTime object (assumes UTC) corresponding to the input string.
		* @return true if the string was converted successfully, false otherwise.
		* @see Parse
		*/
		static bool ParseHttpDate(const std::string& http_data, DateTime& out_datatime);

		/**
		* Gets the local date on this computer.
		*
		* The time component is set to 00:00:00
		*
		* @return Current date.
		* @see Now, UtcNow
		*/
		static DateTime Today()
		{
			return Now().GetDate();
		}

		/**
		* Gets the UTC date and time on this computer.
		*
		* This method returns the Coordinated Universal Time (UTC), which does not take the
		* local computer's time zone and daylight savings settings into account. It should be
		* used when comparing dates and times that should be independent of the user's locale.
		* To get the date and time in the current locale, use Now() instead.
		*
		* @return Current date and time.
		* @see Now
		*/
		static DateTime UtcNow();

		/**
		* Validates the given components of a date and time value.
		*
		* The allow ranges for the components are:
		*		Year: 1 - 9999
		*		Month: 1 - 12
		*		Day: 1 - DaysInMonth(Month)
		*		Hour: 0 - 23
		*		Minute: 0 - 59
		*		Second: 0 - 59
		*		Millisecond: 0 - 999
		*
		* @return true if the components are valid, false otherwise.
		*/
		static bool Validate(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond);

	public:

		/**
		* Serializes the given date and time from or into the specified archive.
		*
		* @param Ar The archive to serialize from or into.
		* @param DateTime The date and time value to serialize.
		* @return The archive.
		*
		* @todo gmp: Figure out better include order in Core.h so this can be inlined.
		*/
		//friend FArchive& operator<<(FArchive& Ar, DateTime& DateTime);

		/**
		* Gets the hash for the specified date and time.
		*
		* @param DateTime The date and time to get the hash for.
		* @return Hash value.
		*/
		friend size_t GetTypeHash(const DateTime& DateTime);
	};

}