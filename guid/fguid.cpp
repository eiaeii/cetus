// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "fguid.h"
#include "stringutil/stringutil.h"
#ifdef _WIN32
#include <combaseapi.h>
#else
#include <uuid/uuid.h>
#endif  // _WIN32
using namespace terra;

std::string FGuid::ToString(EGuidFormats Format) const
{
	switch (Format)
	{
	case EGuidFormats::DigitsWithHyphens:
		return StringUtils::Format("%08X-%04X-%04X-%04X-%04X%08X", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	case EGuidFormats::DigitsWithHyphensInBraces:
		return StringUtils::Format("{%08X-%04X-%04X-%04X-%04X%08X}", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	case EGuidFormats::DigitsWithHyphensInParentheses:
		return StringUtils::Format("(%08X-%04X-%04X-%04X-%04X%08X)", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);

	case EGuidFormats::HexValuesInBraces:
		return StringUtils::Format("{0x%08X,0x%04X,0x%04X,{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}}", A, B >> 16, B & 0xFFFF, C >> 24, (C >> 16) & 0xFF, (C >> 8) & 0xFF, C & 0XFF, D >> 24, (D >> 16) & 0XFF, (D >> 8) & 0XFF, D & 0XFF);

	case EGuidFormats::UniqueObjectGuid:
		return StringUtils::Format("%08X-%08X-%08X-%08X", A, B, C, D);

	default:
		return StringUtils::Format("%08X%08X%08X%08X", A, B, C, D);
	}
}


/* FGuid static interface
*****************************************************************************/

FGuid FGuid::NewGuid()
{
#ifdef _WIN32
	FGuid Result(0, 0, 0, 0); 
	Expects(::CoCreateGuid((GUID*)&Result) == S_OK);
#else
	uuid_t UUID;
	uuid_generate(UUID);

	uint32* Values = (uint32*)(&UUID[0]);
	Result[0] = Values[0];
	Result[1] = Values[1];
	Result[2] = Values[2];
	Result[3] = Values[3];
#endif // _WIN32

	//static uint16_t IncrementCounter = 0;

	//int32_t Year = 0, Month = 0, DayOfWeek = 0, Day = 0, Hour = 0, Min = 0, Sec = 0, MSec = 0; // Use real time for baseline uniqueness
	//uint32_t SequentialBits = static_cast<uint32_t>(IncrementCounter++); // Add sequential bits to ensure sequentially generated guids are unique even if Cycles is wrong
	//uint32_t RandBits = FMath::Rand() & 0xFFFF; // Add randomness to improve uniqueness across machines

	//FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Min, Sec, MSec);

	//Result = FGuid(RandBits | (SequentialBits << 16), Day | (Hour << 8) | (Month << 16) | (Sec << 24), MSec | (Min << 16), Year ^ FPlatformTime::Cycles());
	return Result;
}


bool FGuid::Parse(const std::string& GuidString, FGuid& OutGuid)
{
	if (GuidString.length() == 32)
	{
		return ParseExact(GuidString, EGuidFormats::Digits, OutGuid);
	}

	if (GuidString.length()  == 36)
	{
		return ParseExact(GuidString, EGuidFormats::DigitsWithHyphens, OutGuid);
	}

	if (GuidString.length()  == 38)
	{
		if (StringUtils::StartsWith(GuidString, "{"))
		{
			return ParseExact(GuidString, EGuidFormats::DigitsWithHyphensInBraces, OutGuid);
		}

		return ParseExact(GuidString, EGuidFormats::DigitsWithHyphensInParentheses, OutGuid);
	}

	if (GuidString.length()  == 68)
	{
		return ParseExact(GuidString, EGuidFormats::HexValuesInBraces, OutGuid);
	}

	if (GuidString.length()  == 35)
	{
		return ParseExact(GuidString, EGuidFormats::UniqueObjectGuid, OutGuid);
	}

	return false;
}


bool FGuid::ParseExact(const std::string& GuidString, EGuidFormats Format, FGuid& OutGuid)
{
	std::string NormalizedGuidString;

	NormalizedGuidString.resize(32);

	if (Format == EGuidFormats::Digits)
	{
		NormalizedGuidString = GuidString;
	}
	else if (Format == EGuidFormats::DigitsWithHyphens)
	{
		if ((GuidString[8] != '-') ||
			(GuidString[13] != '-') ||
			(GuidString[18] != '-') ||
			(GuidString[23] != '-'))
		{
			return false;
		}

		NormalizedGuidString += GuidString.substr(0, 8);
		NormalizedGuidString += GuidString.substr(9, 4);
		NormalizedGuidString += GuidString.substr(14, 4);
		NormalizedGuidString += GuidString.substr(19, 4);
		NormalizedGuidString += GuidString.substr(24, 12);
	}
	else if (Format == EGuidFormats::DigitsWithHyphensInBraces)
	{
		if ((GuidString[0] != '{') ||
			(GuidString[9] != '-') ||
			(GuidString[14] != '-') ||
			(GuidString[19] != '-') ||
			(GuidString[24] != '-') ||
			(GuidString[37] != '}'))
		{
			return false;
		}

		NormalizedGuidString += GuidString.substr(1, 8);
		NormalizedGuidString += GuidString.substr(10, 4);
		NormalizedGuidString += GuidString.substr(15, 4);
		NormalizedGuidString += GuidString.substr(20, 4);
		NormalizedGuidString += GuidString.substr(25, 12);
	}
	else if (Format == EGuidFormats::DigitsWithHyphensInParentheses)
	{
		if ((GuidString[0] != '(') ||
			(GuidString[9] != '-') ||
			(GuidString[14] != '-') ||
			(GuidString[19] != '-') ||
			(GuidString[24] != '-') ||
			(GuidString[37] != ')'))
		{
			return false;
		}

		NormalizedGuidString += GuidString.substr(1, 8);
		NormalizedGuidString += GuidString.substr(10, 4);
		NormalizedGuidString += GuidString.substr(15, 4);
		NormalizedGuidString += GuidString.substr(20, 4);
		NormalizedGuidString += GuidString.substr(25, 12);
	}
	else if (Format == EGuidFormats::HexValuesInBraces)
	{
		if ((GuidString[0] != '{') ||
			(GuidString[1] != '0') ||
			(GuidString[2] != 'x') ||
			(GuidString[11] != ',') ||
			(GuidString[12] != '0') ||
			(GuidString[13] != 'x') ||
			(GuidString[18] != ',') ||
			(GuidString[19] != '0') ||
			(GuidString[20] != 'x') ||
			(GuidString[25] != ',') ||
			(GuidString[26] != '{') ||
			(GuidString[27] != '0') ||
			(GuidString[28] != 'x') ||
			(GuidString[31] != ',') ||
			(GuidString[32] != '0') ||
			(GuidString[33] != 'x') ||
			(GuidString[36] != ',') ||
			(GuidString[37] != '0') ||
			(GuidString[38] != 'x') ||
			(GuidString[41] != ',') ||
			(GuidString[42] != '0') ||
			(GuidString[43] != 'x') ||
			(GuidString[46] != ',') ||
			(GuidString[47] != '0') ||
			(GuidString[48] != 'x') ||
			(GuidString[51] != ',') ||
			(GuidString[52] != '0') ||
			(GuidString[53] != 'x') ||
			(GuidString[56] != ',') ||
			(GuidString[57] != '0') ||
			(GuidString[58] != 'x') ||
			(GuidString[61] != ',') ||
			(GuidString[62] != '0') ||
			(GuidString[63] != 'x') ||
			(GuidString[66] != '}') ||
			(GuidString[67] != '}'))
		{
			return false;
		}

		NormalizedGuidString += GuidString.substr(3, 8);
		NormalizedGuidString += GuidString.substr(14, 4);
		NormalizedGuidString += GuidString.substr(21, 4);
		NormalizedGuidString += GuidString.substr(29, 2);
		NormalizedGuidString += GuidString.substr(34, 2);
		NormalizedGuidString += GuidString.substr(39, 2);
		NormalizedGuidString += GuidString.substr(44, 2);
		NormalizedGuidString += GuidString.substr(49, 2);
		NormalizedGuidString += GuidString.substr(54, 2);
		NormalizedGuidString += GuidString.substr(59, 2);
		NormalizedGuidString += GuidString.substr(64, 2);
	}
	else if (Format == EGuidFormats::UniqueObjectGuid)
	{
		if ((GuidString[8] != '-') ||
			(GuidString[17] != '-') ||
			(GuidString[26] != '-'))
		{
			return false;
		}

		NormalizedGuidString += GuidString.substr(0, 8);
		NormalizedGuidString += GuidString.substr(9, 8);
		NormalizedGuidString += GuidString.substr(18, 8);
		NormalizedGuidString += GuidString.substr(27, 8);
	}

	for (size_t Index = 0; Index < NormalizedGuidString.length() ; ++Index)
	{
		
		if (::isxdigit((unsigned char)NormalizedGuidString[Index]) == 0)
		{
			return false;
		}
	}

	OutGuid = FGuid(
		Parse::HexNumber(NormalizedGuidString.substr(0, 8).c_str()),
		Parse::HexNumber(NormalizedGuidString.substr(8, 8).c_str()),
		Parse::HexNumber(NormalizedGuidString.substr(16, 8).c_str()),
		Parse::HexNumber(NormalizedGuidString.substr(24, 8).c_str())
	);

	return true;
}
