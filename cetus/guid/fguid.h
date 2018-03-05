#pragma once

// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "core.h"
#include "gsl_assert.h"

namespace terra
{
	/**
	* Enumerates known GUID formats.
	*/
	enum class EGuidFormats
	{
		/**
		* 32 digits.
		*
		* For example: "00000000000000000000000000000000"
		*/
		Digits,

		/**
		* 32 digits separated by hyphens.
		*
		* For example: 00000000-0000-0000-0000-000000000000
		*/
		DigitsWithHyphens,

		/**
		* 32 digits separated by hyphens and enclosed in braces.
		*
		* For example: {00000000-0000-0000-0000-000000000000}
		*/
		DigitsWithHyphensInBraces,

		/**
		* 32 digits separated by hyphens and enclosed in parentheses.
		*
		* For example: (00000000-0000-0000-0000-000000000000)
		*/
		DigitsWithHyphensInParentheses,

		/**
		* Comma-separated hexadecimal values enclosed in braces.
		*
		* For example: {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
		*/
		HexValuesInBraces,

		/**
		* This format is currently used by the FUniqueObjectGuid class.
		*
		* For example: 00000000-00000000-00000000-00000000
		*/
		UniqueObjectGuid,
	};

	/**
	* Implements a globally unique identifier.
	*/
	struct FGuid
	{
	public:

		/** Default constructor. */
		FGuid()
			: A(0)
			, B(0)
			, C(0)
			, D(0)
		{ }

		/**
		* Creates and initializes a new GUID from the specified components.
		*
		* @param InA The first component.
		* @param InB The second component.
		* @param InC The third component.
		* @param InD The fourth component.
		*/
		FGuid(uint32_t InA, uint32_t InB, uint32_t InC, uint32_t InD)
			: A(InA), B(InB), C(InC), D(InD)
		{ }

	public:

		/**
		* Compares two GUIDs for equality.
		*
		* @param X The first GUID to compare.
		* @param Y The second GUID to compare.
		* @return true if the GUIDs are equal, false otherwise.
		*/
		friend bool operator==(const FGuid& X, const FGuid& Y)
		{
			return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
		}

		/**
		* Compares two GUIDs for inequality.
		*
		* @param X The first GUID to compare.
		* @param Y The second GUID to compare.
		* @return true if the GUIDs are not equal, false otherwise.
		*/
		friend bool operator!=(const FGuid& X, const FGuid& Y)
		{
			return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
		}

		/**
		* Compares two GUIDs.
		*
		* @param X The first GUID to compare.
		* @param Y The second GUID to compare.
		* @return true if the first GUID is less than the second one.
		*/
		friend bool operator<(const FGuid& X, const FGuid& Y)
		{
			return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
				((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
				((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
					((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false)))))))); //-V583
		}

		/**
		* Provides access to the GUIDs components.
		*
		* @param Index The index of the component to return (0...3).
		* @return The component.
		*/
		uint32_t& operator[](int32_t Index)
		{
			Expects(Index >= 0);
			Expects(Index < 4);

			switch (Index)
			{
			case 0: return A;
			case 1: return B;
			case 2: return C;
			case 3: return D;
			}

			return A;
		}

		/**
		* Provides read-only access to the GUIDs components.
		*
		* @param Index The index of the component to return (0...3).
		* @return The component.
		*/
		const uint32_t& operator[](int32_t Index) const
		{
			Expects(Index >= 0);
			Expects(Index < 4);

			switch (Index)
			{
			case 0: return A;
			case 1: return B;
			case 2: return C;
			case 3: return D;
			}

			return A;
		}

		/**
		* Serializes a GUID from or into an archive.
		*
		* @param Ar The archive to serialize from or into.
		* @param G The GUID to serialize.
		*/
		//friend FArchive& operator<<(FArchive& Ar, FGuid& G)
		//{
		//	return Ar << G.A << G.B << G.C << G.D;
		//}

		//bool Serialize(FArchive& Ar)
		//{
		//	Ar << *this;
		//	return true;
		//}

	public:
		/**
		* Invalidates the GUID.
		*
		* @see IsValid
		*/
		void Invalidate()
		{
			A = B = C = D = 0;
		}

		/**
		* Checks whether this GUID is valid or not.
		*
		* A GUID that has all its components set to zero is considered invalid.
		*
		* @return true if valid, false otherwise.
		* @see Invalidate
		*/
		bool IsValid() const
		{
			return ((A | B | C | D) != 0);
		}

		/**
		* Converts this GUID to its string representation.
		*
		* @return The string representation.
		*/
		std::string ToString() const
		{
			return ToString(EGuidFormats::Digits);
		}

		/**
		* Converts this GUID to its string representation using the specified format.
		*
		* @param Format The string format to use.
		* @return The string representation.
		*/
		std::string ToString(EGuidFormats Format) const;
	public:

		/**
		* Returns a new GUID.
		*
		* @return A new GUID.
		*/
		static FGuid NewGuid();

		/**
		* Converts a string to a GUID.
		*
		* @param GuidString The string to convert.
		* @param OutGuid Will contain the parsed GUID.
		* @return true if the string was converted successfully, false otherwise.
		* @see ParseExact, ToString
		*/
		static bool Parse(const std::string& GuidString, FGuid& OutGuid);

		/**
		* Converts a string with the specified format to a GUID.
		*
		* @param GuidString The string to convert.
		* @param Format The string format to parse.
		* @param OutGuid Will contain the parsed GUID.
		* @return true if the string was converted successfully, false otherwise.
		* @see Parse, ToString
		*/
		static bool ParseExact(const std::string& GuidString, EGuidFormats Format, FGuid& OutGuid);

		//private:
	public:

		/** Holds the first component. */
		uint32_t A;

		/** Holds the second component. */
		uint32_t B;

		/** Holds the third component. */
		uint32_t C;

		/** Holds the fourth component. */
		uint32_t D;
	};

	namespace Lex
	{
		/**
		* Guid default string conversion.
		*/
		inline std::string ToString(const FGuid& Value)
		{
			return Value.ToString();
		}
	}

	class Parse
	{
	public:
		static int32_t HexDigit(char c)
		{
			int32_t Result = 0;

			if (c >= '0' && c <= '9')
			{
				Result = c - '0';
			}
			else if (c >= 'a' && c <= 'f')
			{
				Result = c + 10 - 'a';
			}
			else if (c >= 'A' && c <= 'F')
			{
				Result = c + 10 - 'A';
			}
			else
			{
				Result = 0;
			}

			return Result;
		}

		static  uint32_t HexNumber(const char* HexString)
		{
			uint32_t Ret = 0;

			while (*HexString)
			{
				Ret *= 16;
				Ret += Parse::HexDigit(*HexString++);
			}

			return Ret;
		}
	};

	static_assert(std::is_standard_layout<FGuid>::value, "not pod type!");
}