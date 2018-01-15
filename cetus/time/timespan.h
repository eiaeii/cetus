#pragma once

#include "cetus_define.h"

namespace terra
{
	namespace ETimespan
	{
		/** The number of timespan ticks per day. */
		const int64_t TicksPerDay = 864000000000;

		/** The number of timespan ticks per hour. */
		const int64_t TicksPerHour = 36000000000;

		/** The number of timespan ticks per microsecond. */
		const int64_t TicksPerMicrosecond = 10;

		/** The number of timespan ticks per millisecond. */
		const int64_t TicksPerMillisecond = 10000;

		/** The number of timespan ticks per minute. */
		const int64_t TicksPerMinute = 600000000;

		/** The number of timespan ticks per second. */
		const int64_t TicksPerSecond = 10000000;

		/** The number of timespan ticks per week. */
		const int64_t TicksPerWeek = 6048000000000;
	}

	/**
	* Implements a time span.
	*
	* A time span is the difference between two dates and times. For example, the time span between
	* 12:00:00 January 1, 2000 and 18:00:00 January 2, 2000 is 30.0 hours. Time spans are measured in
	* positive or negative ticks depending on whether the difference is measured forward or backward.
	* Each tick has a resolution of 0.1 microseconds (= 100 nanoseconds).
	*
	* In conjunction with the companion class DateTime, time spans can be used to perform date and time
	* based arithmetic, such as calculating the difference between two dates or adding a certain amount
	* of time to a given date.
	*
	* @see DateTime
	*/
	struct Timespan
	{
	private:

		/** Holds the time span in 100 nanoseconds resolution. */
		int64_t ticks_;
	public:

		/** Default constructor (no initialization). */
		Timespan() { }

		/**
		* Creates and initializes a new time interval with the specified number of ticks.
		*
		* @param Ticks The number of ticks.
		*/
		Timespan(int64_t ticks)
			: ticks_(ticks)
		{ }

		/**
		* Creates and initializes a new time interval with the specified number of hours, minutes and seconds.
		*
		* @param Hours The hours component.
		* @param Minutes The minutes component.
		* @param Seconds The seconds component.
		*/
		Timespan(int32_t hours, int32_t minutes, int32_t seconds)
		{
			Assign(0, hours, minutes, seconds, 0, 0);
		}

		/**
		* Creates and initializes a new time interval with the specified number of days, hours, minutes and seconds.
		*
		* @param Days The days component.
		* @param Hours The hours component.
		* @param Minutes The minutes component.
		* @param Seconds The seconds component.
		*/
		Timespan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds)
		{
			Assign(days, hours, minutes, seconds, 0, 0);
		}

		/**
		* Creates and initializes a new time interval with the specified number of days, hours, minutes and seconds.
		*
		* @param Days The days component.
		* @param Hours The hours component.
		* @param Minutes The minutes component.
		* @param Seconds The seconds component.
		* @param Milliseconds The milliseconds component.
		* @param Microseconds The microseconds component (default = 0).
		*/
		Timespan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds = 0)
		{
			Assign(days, hours, minutes, seconds, milliseconds, microseconds);
		}

	public:

		/**
		* Returns result of adding the given time span to this time span.
		*
		* @return A time span whose value is the sum of this time span and the given time span.
		*/
		Timespan operator+(const Timespan& other) const
		{
			return Timespan(ticks_ + other.ticks_);
		}

		/**
		* Adds the given time span to this time span.
		*
		* @return This time span.
		*/
		Timespan& operator+=(const Timespan& other)
		{
			ticks_ += other.ticks_;
			return *this;
		}

		/**
		* Returns the inverse of this time span.
		*
		* The value of this time span must be greater than FTimespan::MinValue(), or else an overflow will occur.
		*
		* @return Inverse of this time span.
		*/
		Timespan operator-() const
		{
			return Timespan(-ticks_);
		}

		/**
		* Returns the result of subtracting the given time span from this time span.
		*
		* @param other The time span to compare with.
		* @return A time span whose value is the difference of this time span and the given time span.
		*/
		Timespan operator-(const Timespan& other) const
		{
			return Timespan(ticks_ - other.ticks_);
		}

		/**
		* Subtracts the given time span from this time span.
		*
		* @param other The time span to subtract.
		* @return This time span.
		*/
		Timespan& operator-=(const Timespan& other)
		{
			ticks_ -= other.ticks_;
			return *this;
		}

		/**
		* Returns the result of multiplying the this time span with the given scalar.
		*
		* @param Scalar The scalar to multiply with.
		* @return A time span whose value is the product of this time span and the given scalar.
		*/
		Timespan operator*(float scalar) const
		{
			return Timespan((int64_t)(ticks_ * scalar));
		}

		/**
		* Multiplies this time span with the given scalar.
		*
		* @param Scalar The scalar to multiply with.
		* @return This time span.
		*/
		Timespan& operator*=(float scalar)
		{
			ticks_ = (int64_t)(ticks_ * scalar);
			return *this;
		}

		/**
		* Returns the result of calculating the modulus of this time span with another time span.
		*
		* @param other The time span to divide by.
		* @return A time span representing the remainder of the modulus operation.
		*/
		Timespan operator%(const Timespan& other) const
		{
			return Timespan(ticks_ % other.ticks_);
		}

		/**
		* Calculates this time span modulo another.
		*
		* @param other The time span to divide by.
		* @return This time span.
		*/
		Timespan& operator%=(const Timespan& other)
		{
			ticks_ = ticks_ % other.ticks_;
			return *this;
		}

		/**
		* Compares this time span with the given time span for equality.
		*
		* @param other The time span to compare with.
		* @return true if the time spans are equal, false otherwise.
		*/
		bool operator==(const Timespan& other) const
		{
			return (ticks_ == other.ticks_);
		}

		/**
		* Compares this time span with the given time span for inequality.
		*
		* @param other The time span to compare with.
		* @return true if the time spans are not equal, false otherwise.
		*/
		bool operator!=(const Timespan& other) const
		{
			return (ticks_ != other.ticks_);
		}

		/**
		* Checks whether this time span is greater than the given time span.
		*
		* @param other The time span to compare with.
		* @return true if this time span is greater, false otherwise.
		*/
		bool operator>(const Timespan& other) const
		{
			return (ticks_ > other.ticks_);
		}

		/**
		* Checks whether this time span is greater than or equal to the given time span.
		*
		* @param other The time span to compare with.
		* @return true if this time span is greater or equal, false otherwise.
		*/
		bool operator>=(const Timespan& other) const
		{
			return (ticks_ >= other.ticks_);
		}

		/**
		* Checks whether this time span is less than the given time span.
		*
		* @param other The time span to compare with.
		* @return true if this time span is less, false otherwise.
		*/
		bool operator<(const Timespan& other) const
		{
			return (ticks_ < other.ticks_);
		}

		/**
		* Checks whether this time span is less than or equal to the given time span.
		*
		* @param other The time span to compare with.
		* @return true if this time span is less or equal, false otherwise.
		*/
		bool operator<=(const Timespan& other) const
		{
			return (ticks_ <= other.ticks_);
		}

	public:
		/**
		* Gets the days component of this time span.
		*
		* @return Days component.
		*/
		int32_t GetDays() const
		{
			return (int32_t)(ticks_ / ETimespan::TicksPerDay);
		}

		/**
		* Returns a time span with the absolute value of this time span.
		*
		* This method may overflow the timespan if its value is equal to MinValue.
		*
		* @return Duration of this time span.
		* @see MinValue
		*/
		Timespan GetDuration()
		{
			return Timespan(ticks_ >= 0 ? ticks_ : -ticks_);
		}

		/**
		* Gets the hours component of this time span.
		*
		* @return Hours component.
		* @see GetTotalHours
		*/
		int32_t GetHours() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerHour) % 24);
		}

		/**
		* Gets the microseconds component of this time span.
		*
		* @return Microseconds component.
		* @see GetTotalMicroseconds
		*/
		int32_t GetMicroseconds() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerMicrosecond) % 1000);
		}

		/**
		* Gets the milliseconds component of this time span.
		*
		* @return Milliseconds component.
		* @see GetTotalMilliseconds
		*/
		int32_t GetMilliseconds() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerMillisecond) % 1000);
		}

		/**
		* Gets the minutes component of this time span.
		*
		* @return Minutes component.
		* @see GetTotalMinutes
		*/
		int32_t GetMinutes() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerMinute) % 60);
		}

		/**
		* Gets the seconds component of this time span.
		*
		* @return Seconds component.
		* @see GetTotalSeconds
		*/
		int32_t GetSeconds() const
		{
			return (int32_t)((ticks_ / ETimespan::TicksPerSecond) % 60);
		}

		/**
		* Gets the number of ticks represented by this time span.
		*
		* @return Number of ticks.
		*/
		int64_t GetTicks() const
		{
			return ticks_;
		}

		/**
		* Gets the total number of days represented by this time span.
		*
		* @return Number of days.
		* @see GetDays
		*/
		double GetTotalDays() const
		{
			return ((double)ticks_ / ETimespan::TicksPerDay);
		}

		/**
		* Gets the total number of hours represented by this time span.
		*
		* @return Number of hours.
		* @see GetHours
		*/
		double GetTotalHours() const
		{
			return ((double)ticks_ / ETimespan::TicksPerHour);
		}

		/**
		* Gets the total number of microseconds represented by this time span.
		*
		* @return Number of microseconds.
		* @see GetMicroseconds
		*/
		double GetTotalMicroseconds() const
		{
			return ((double)ticks_ / ETimespan::TicksPerMicrosecond);
		}

		/**
		* Gets the total number of milliseconds represented by this time span.
		*
		* @return Number of milliseconds.
		* @see GetMilliseconds
		*/
		double GetTotalMilliseconds() const
		{
			return ((double)ticks_ / ETimespan::TicksPerMillisecond);
		}

		/**
		* Gets the total number of minutes represented by this time span.
		*
		* @return Number of minutes.
		* @see GetMinutes
		*/
		double GetTotalMinutes() const
		{
			return ((double)ticks_ / ETimespan::TicksPerMinute);
		}

		/**
		* Gets the total number of seconds represented by this time span.
		*
		* @return Number of seconds.
		* @see GetSeconds
		*/
		double GetTotalSeconds() const
		{
			return ((double)ticks_ / ETimespan::TicksPerSecond);
		}

		/**
		* Check whether this time span is zero.
		*
		* @return true if the time span is zero, false otherwise.
		* @see Zero
		*/
		bool IsZero() const
		{
			return (ticks_ == 0LL);
		}

		/**
		* Serializes this time span from or into the specified archive.
		*
		* @param Ar The archive to serialize from or into.
		* @return true on success, false otherwise.
		*/
		//bool Serialize(FArchive& Ar);

		/**
		* Returns the string representation of this time span using a default format.
		*
		* The returned string has the following format:
		*		[-][d.]hh:mm:ss.fff
		*
		* @return String representation.
		* @see Parse
		*/
		std::string ToString() const;

		/**
		* Converts this time span to its string representation.
		*
		* The following formatting codes are available:
		*		%n - prints the minus sign (for negative time spans only)
		*		%N - prints the minus or plus sign (always)
		*		%d - prints the time span's days part
		*		%h - prints the time span's hours part (0..23)
		*		%m - prints the time span's minutes part (0..59)
		*		%s - prints the time span's seconds part (0..59)
		*		%f - prints the time span's milliseconds part (0..999)
		*		%D - prints the total number of days (without minus sign)
		*		%H - prints the total number of hours (without minus sign)
		*		%M - prints the total number of minutes (without minus sign)
		*		%S - prints the total number of seconds (without minus sign)
		*		%F - prints the total number of milliseconds (without minus sign)
		*
		* @param Format - The format of the returned string.
		* @return String representation.
		* @see Parse
		*/
		std::string ToString(const char* Format) const;

	public:

		/**
		* Creates a time span that represents the specified number of days.
		*
		* @param Days The number of days.
		* @return Time span.
		* @see FromHours, FromMicroseconds, FromMilliseconds, FromMinutes, FromSeconds
		*/
		static Timespan FromDays(double Days);

		/**
		* Creates a time span that represents the specified number of hours.
		*
		* @param Hours The number of hours.
		* @return Time span.
		* @see FromDays, FromMicroseconds, FromMilliseconds, FromMinutes, FromSeconds
		*/
		static Timespan FromHours(double Hours);

		/**
		* Creates a time span that represents the specified number of microseconds.
		*
		* @param Microseconds The number of microseconds.
		* @return Time span.
		* @see FromDays, FromHours, FromMinutes, FromSeconds, FromMilliseconds
		*/
		static Timespan FromMicroseconds(double Microseconds);

		/**
		* Creates a time span that represents the specified number of milliseconds.
		*
		* @param Milliseconds The number of milliseconds.
		* @return Time span.
		* @see FromDays, FromHours, FromMicroseconds, FromMinutes, FromSeconds
		*/
		static Timespan FromMilliseconds(double Milliseconds);

		/**
		* Creates a time span that represents the specified number of minutes.
		*
		* @param Minutes The number of minutes.
		* @return Time span.
		* @see FromDays, FromHours, FromMicroseconds, FromMilliseconds, FromSeconds
		*/
		static Timespan FromMinutes(double Minutes);

		/**
		* Creates a time span that represents the specified number of seconds.
		*
		* @param Seconds The number of seconds.
		* @return Time span.
		* @see FromDays, FromHours, FromMicroseconds, FromMilliseconds, FromMinutes
		*/
		static Timespan FromSeconds(double Seconds);

		/**
		* Returns the maximum time span value.
		*
		* The maximum time span value is slightly more than 10,675,199 days.
		*
		* @return Maximum time span.
		* @see MinValue,Zero
		*/
		static Timespan MaxValue()
		{
			return Timespan(9223372036854775807);
		}

		/**
		* Returns the minimum time span value.
		*
		* The minimum time span value is slightly less than -10,675,199 days.
		*
		* @return Minimum time span.
		* @see MaxValue, ZeroValue
		*/
		static Timespan MinValue()
		{
			return Timespan(-9223372036854775807 - 1);
		}

		/**
		* Converts a string to a time span.
		*
		* Currently, the string must be in the format written by FTimespan.ToString().
		* other formats are not supported at this time.
		*
		* @param TimespanString The string to convert.
		* @param outTimespan Will contain the parsed time span.
		* @return true if the string was converted successfully, false otherwise.
		* @see ToString
		*/
		static bool Parse(const std::string& TimespanString, Timespan& outTimespan);

		/**
		* Returns the zero time span value.
		*
		* The zero time span value can be used in comparison operations with other time spans.
		*
		* @return Zero time span.
		* @see IsZero, MaxValue, MinValue
		*/
		static Timespan Zero()
		{
			return Timespan(0);
		}

	public:

		/**
		* Serializes the given time span from or into the specified archive.
		*
		* @param Ar The archive to serialize from or into.
		* @param Timespan The time span value to serialize.
		* @return The archive.
		*
		* @todo gmp: Figure out better include order in Core.h so this can be inlined.
		*/
		//friend FArchive& operator<<(FArchive& Ar, Timespan& timespan);

		/**
		* Gets the hash for the specified time span.
		*
		* @param Timespan The timespan to get the hash for.
		* @return Hash value.
		*/
		friend uint32_t GetTypeHash(const Timespan& timespan);

	protected:

		/**
		* Assigns the specified components to this time span.
		*
		* @param Days The days component.
		* @param Hours The hours component.
		* @param Minutes The minutes component.
		* @param Seconds The seconds component.
		* @param Milliseconds The milliseconds component.
		* @param Microseconds The microseconds component.
		*/
		void Assign(int32_t Days, int32_t Hours, int32_t Minutes, int32_t Seconds, int32_t Milliseconds, int32_t Microseconds);

	private:

	};


	/**
	* Pre-multiplies a time span with the given scalar.
	*
	* @param Scalar The scalar to pre-multiply with.
	* @param Timespan The time span to multiply.
	*/
	inline Timespan operator*(float scalar, const Timespan& timespan)
	{
		return timespan.operator*(scalar);
	}

}