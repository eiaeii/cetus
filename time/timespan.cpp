#include "timespan.h"
#include "stringutil/stringutil.h"
#include <vector>

//bool Timespan::Serialize(FArchive& Ar)
//{
//	Ar << *this;
//
//	return true;
//}


using namespace terra;

std::string Timespan::ToString() const
{
	if (GetDays() == 0)
	{
		return ToString("%n%h:%m:%s.%f");
	}

	return ToString("%n%d.%h:%m:%s.%f");
}

std::string Timespan::ToString(const char* fmt) const
{
	std::string result;
	while (*fmt != '\0')
	{
		if ((*fmt == '%') && (*++fmt != '\0'))
		{
			switch (*fmt)
			{
			case 'n': if (ticks_ < 0) result += '-'; break;
			case 'N': result += (ticks_ < 0) ? '-' : '+'; break;
			case 'd': result += StringUtils::Format("%i", std::abs(GetDays())); break;
			case 'h': result += StringUtils::Format("%02i", std::abs(GetHours())); break;
			case 'm': result += StringUtils::Format("%02i", std::abs(GetMinutes())); break;
			case 's': result += StringUtils::Format("%02i", std::abs(GetSeconds())); break;
			case 'f': result += StringUtils::Format("%03i", std::abs(GetMilliseconds())); break;
			case 'D': result += StringUtils::Format("%f", std::abs(GetTotalDays())); break;
			case 'H': result += StringUtils::Format("%f", std::abs(GetTotalHours())); break;
			case 'M': result += StringUtils::Format("%f", std::abs(GetTotalMinutes())); break;
			case 'S': result += StringUtils::Format("%f", std::abs(GetTotalSeconds())); break;
			case 'F': result += StringUtils::Format("%f", std::abs(GetTotalMilliseconds())); break;

			default:

				result += *fmt;
			}
		}
		else
		{
			result += *fmt;
		}

		++fmt;
	}

	return result;
}


/* FTimespan static interface
*****************************************************************************/

Timespan Timespan::FromDays(double days)
{
	Expects((days >= MinValue().GetTotalDays()) && (days <= MaxValue().GetTotalDays()));

	return Timespan(static_cast<int64_t>(days * ETimespan::TicksPerDay));
}


Timespan Timespan::FromHours(double hours)
{
	Expects((hours >= MinValue().GetTotalHours()) && (hours <= MaxValue().GetTotalHours()));

	return Timespan(static_cast<int64_t>(hours * ETimespan::TicksPerHour));
}


Timespan Timespan::FromMicroseconds(double microseconds)
{
	Expects((microseconds >= MinValue().GetTotalMicroseconds()) && (microseconds <= MaxValue().GetTotalMicroseconds()));

	return Timespan(static_cast<int64_t>(microseconds * ETimespan::TicksPerMicrosecond));
}


Timespan Timespan::FromMilliseconds(double milliseconds)
{
	Expects((milliseconds >= MinValue().GetTotalMilliseconds()) && (milliseconds <= MaxValue().GetTotalMilliseconds()));

	return Timespan(static_cast<int64_t>(milliseconds * ETimespan::TicksPerMillisecond));
}


Timespan Timespan::FromMinutes(double minutes)
{
	Expects((minutes >= MinValue().GetTotalMinutes()) && (minutes <= MaxValue().GetTotalMinutes()));

	return Timespan(static_cast<int64_t>(minutes * ETimespan::TicksPerMinute));
}


Timespan Timespan::FromSeconds(double seconds)
{
	Expects((seconds >= MinValue().GetTotalSeconds()) && (seconds <= MaxValue().GetTotalSeconds()));

	return Timespan(static_cast<int64_t>(seconds * ETimespan::TicksPerSecond));
}


bool Timespan::Parse(const std::string& in_timespan_string, Timespan& out_timespan)
{
	// @todo gmp: implement stricter FTimespan parsing; this implementation is too forgiving.
	std::string token_string = in_timespan_string;
	StringUtils::Replace(token_string, '.', ':');
	StringUtils::Replace(token_string, ',', ':');

	bool is_negative = StringUtils::StartsWith(token_string, "-");
	StringUtils::Replace(token_string, '-', ':');

	std::vector<std::string> tokens;
	StringUtils::Split(token_string, ":", tokens);

	if (tokens.size() == 4)
	{
		tokens.emplace_back(std::string("0"));
	}

	if (tokens.size() != 5)
	{
		return false;
	}

	out_timespan.Assign(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()),
		atoi(tokens[2].c_str()), atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), 0);

	if (is_negative)
	{
		out_timespan.ticks_ *= -1;
	}
	return true;
}


/* FTimespan friend functions
*****************************************************************************/

//FArchive& operator<<(FArchive& Ar, Timespan& Timespan)
//{
//	return Ar << Timespan.ticks_;
//}


//uint32_t GetTypeHash(const Timespan& timespan)
//{
//	return std::hash<int64_t>()(timespan.ticks_);
//}


/* FTimespan implementation
*****************************************************************************/

void Timespan::Assign(int32_t Days, int32_t Hours, int32_t Minutes, int32_t Seconds, int32_t Milliseconds, int32_t Microseconds)
{
	int64_t TotalTicks = ETimespan::TicksPerMicrosecond * (1000 * (1000 * (60 * 60 * 24 * (int64_t)Days + 60 * 60 * (int64_t)Hours + 60 * (int64_t)Minutes + (int64_t)Seconds) + (int64_t)Milliseconds) + (int64_t)Microseconds);
	Expects((TotalTicks >= MinValue().GetTicks()) && (TotalTicks <= MaxValue().GetTicks()));

	ticks_ = TotalTicks;
}
