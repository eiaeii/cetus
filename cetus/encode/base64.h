#pragma once

#include "cetus_define.h"
#include <vector>

namespace cetus
{
	/**
	* Class for encoding/decoding Base64 data (RFC 4648)
	*/
	class Base64Encoding
	{
		/**
		* Base64 supports encoding any 3 uint8_t chunk of binary data into a 4 uint8_t set of characters from the alphabet below
		*/
		static uint8_t kEncodingAlphabet[64];
		/**
		* Used to do a reverse look up from the encoded alphabet to the original 6 bit value
		*/
		static uint8_t kDecodingAlphabet[256];

		/** Ctor hidden on purpose, use static methods only */
		Base64Encoding()
		{
		}

		/**
		* Encodes the source into a Base64 string
		*
		* @param Source the binary payload to stringify
		* @param Length the length of the payload that needs encoding
		*
		* @return the stringified form of the binary data
		*/
		static std::string Encode(uint8_t* source, uint32_t length);

		/**
		* Decodes a Base64 string into an array of bytes
		*
		* @param Source the stringified data to convert
		* @param Length the length of the buffer being converted
		* @param Dest the out buffer that will be filled with the decoded data
		* @param PadCount the out count of the padding on the orignal buffer (0 to 2)
		*
		* @return TRUE if the buffer was decoded, FALSE if it failed to decode
		*/
		static bool Decode(const char* source, uint32_t length, uint8_t* dest, uint32_t& pad_count);

	public:
		/**
		* Encodes a binary uint8_t array into a Base64 string
		*
		* @param Source the binary data to convert
		*
		* @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
		*/
		static std::string Encode(const std::vector<uint8_t>& source);

		/**
		* Decodes a Base64 string into an array of bytes
		*
		* @param Source the stringified data to convert
		* @param Dest the out buffer that will be filled with the decoded data
		*/
		static bool Decode(const std::string& source, std::vector<uint8_t>& dest);

		/**
		* Encodes a std::string into a Base64 string
		*
		* @param Source the string data to convert
		*
		* @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
		*/
		static std::string Encode(const std::string& source);

		/**
		* Decodes a Base64 string into a std::string
		*
		* @param Source the stringified data to convert
		* @param Dest the out buffer that will be filled with the decoded data
		*/
		static bool Decode(const std::string& source, std::string& dest);
	};
}