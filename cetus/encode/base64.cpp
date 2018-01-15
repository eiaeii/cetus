#include "base64.h"

using namespace terra;

/** The table used to encode a 6 bit value as an ascii character */
uint8_t Base64Encoding::kEncodingAlphabet[64] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e',
'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

/** The table used to convert an ascii character into a 6 bit value */
uint8_t Base64Encoding::kDecodingAlphabet[256] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22,
0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

/**
* Encodes a binary uint8_t array into a Base64 string
*
* @param Source the binary data to convert
*
* @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
*/
std::string Base64Encoding::Encode(const std::vector<uint8_t>& source)
{
	return Encode((uint8_t*)source.data(), (uint32_t)source.size());
}

/**
* Decodes a Base64 string into an array of bytes
*
* @param Source the stringified data to convert
* @param Dest the out buffer that will be filled with the decoded data
*
* @return true if the buffer was decoded, false if it failed to decode
*/
bool Base64Encoding::Decode(const std::string& source, std::vector<uint8_t>& dest)
{
	uint32_t len = static_cast<uint32_t>(source.length());
	// Size must be a multiple of 4
	if (len % 4)
	{
		return false;
	}
	// Each 4 uint8_t chunk of characters is 3 bytes of data
	uint32_t expected_length = len / 4 * 3;
	// Add the number we need for output
	dest.reserve(expected_length);
	uint8_t* buffer = dest.data();
	uint32_t pad_count = 0;
	bool success = Decode(source.c_str(), len, dest.data(), pad_count);
	if (success)
	{
		if (pad_count > 0)
		{
			dest.erase(dest.begin() + (expected_length - pad_count), dest.end());
		}
	}
	return success;
}

/**
* Encodes a std::string into a Base64 string
*
* @param Source the string data to convert
*
* @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
*/
std::string Base64Encoding::Encode(const std::string& source)
{
	return Encode((uint8_t*)(source.c_str()), (uint32_t)source.length());
}

/**
* Decodes a Base64 string into a std::string
*
* @param Source the stringified data to convert
* @param Dest the out buffer that will be filled with the decoded data
*/
bool Base64Encoding::Decode(const std::string& source, std::string& dest)
{
	uint32_t len = static_cast<uint32_t>(source.length());
	// Size must be a multiple of 4
	if (len % 4)
	{
		return false;
	}
	// Each 4 uint8_t chunk of characters is 3 bytes of data
	uint32_t expected_length = len / 4 * 3;
	std::vector<char> temp_dest;
	temp_dest.reserve(expected_length);
	uint8_t* buffer = (uint8_t*)temp_dest.data();
	uint32_t pad_count = 0;

	bool success = Decode(source.c_str(), len, buffer, pad_count);
	if (success)
	{
		if (pad_count > 0)
		{
			buffer[expected_length - pad_count] = 0;
		}
		else
		{
			temp_dest.push_back('\0');
		}
		dest = (temp_dest.data());
	}
	return success;
}

/**
* Encodes the source into a Base64 string
*
* @param Source the binary payload to stringify
* @param Length the length of the payload that needs encoding
*
* @return the stringified form of the binary data
*/
std::string Base64Encoding::Encode(uint8_t* source, uint32_t length)
{
	// Each 3 uint8_t set of data expands to 4 bytes and must be padded to 4 bytes
	uint32_t expected_length = (length + 2) / 3 * 4;
	std::string out_buffer;
	out_buffer.reserve(expected_length);

	char encoded_bytes[5];
	// Null terminate this so we can append it easily as a string
	encoded_bytes[4] = 0;
	// Loop through the buffer converting 3 bytes of binary data at a time
	while (length >= 3)
	{
		uint8_t a = *source++;
		uint8_t b = *source++;
		uint8_t c = *source++;
		length -= 3;
		// The algorithm takes 24 bits of data (3 bytes) and breaks it into 4 6bit chunks represented as ascii
		uint32_t byte_triplet = a << 16 | b << 8 | c;
		// Use the 6bit block to find the representation ascii character for it
		encoded_bytes[3] = kEncodingAlphabet[byte_triplet & 0x3F];
		// Shift each time to get the next 6 bit chunk
		byte_triplet >>= 6;
		encoded_bytes[2] = kEncodingAlphabet[byte_triplet & 0x3F];
		byte_triplet >>= 6;
		encoded_bytes[1] = kEncodingAlphabet[byte_triplet & 0x3F];
		byte_triplet >>= 6;
		encoded_bytes[0] = kEncodingAlphabet[byte_triplet & 0x3F];
		// Now we can append this buffer to our destination string
		out_buffer += encoded_bytes;
	}
	// Since this algorithm operates on blocks, we may need to pad the last chunks
	if (length > 0)
	{
		uint8_t a = *source++;
		uint8_t b = 0;
		uint8_t c = 0;
		// Grab the second character if it is a 2 uint8_t finish
		if (length == 2)
		{
			b = *source;
		}
		uint32_t byte_triplet = a << 16 | b << 8 | c;
		// Pad with = to make a 4 uint8_t chunk
		encoded_bytes[3] = '=';
		byte_triplet >>= 6;
		// If there's only one 1 uint8_t left in the source, then you need 2 pad chars
		if (length == 1)
		{
			encoded_bytes[2] = '=';
		}
		else
		{
			encoded_bytes[2] = kEncodingAlphabet[byte_triplet & 0x3F];
		}
		// Now encode the remaining bits the same way
		byte_triplet >>= 6;
		encoded_bytes[1] = kEncodingAlphabet[byte_triplet & 0x3F];
		byte_triplet >>= 6;
		encoded_bytes[0] = kEncodingAlphabet[byte_triplet & 0x3F];
		out_buffer += encoded_bytes;
	}
	return out_buffer;
}

/**
* Decodes a Base64 string into an array of bytes
*
* @param Source the stringified data to convert
* @param Length the length of the buffer being converted
* @param Dest the out buffer that will be filled with the decoded data
* @param PadCount the out count of the padding on the orignal buffer (0 to 2)
*
* @return true if the buffer was decoded, false if it failed to decode
*/
bool Base64Encoding::Decode(const char* source, uint32_t length, uint8_t* dest, uint32_t& pad_count)
{
	pad_count = 0;
	uint8_t decoded_values[4];
	while (length)
	{
		// Decode the next 4 BYTEs
		for (int32_t idx = 0; idx < 4; idx++)
		{
			// Tell the caller if there were any pad bytes
			if (*source == '=')
			{
				pad_count++;
			}
			decoded_values[idx] = kDecodingAlphabet[(int32_t)(*source++)];
			// Abort on values that we don't understand
			if (decoded_values[idx] == 0xFF)
			{
				return false;
			}
		}
		length -= 4;
		// Rebuild the original 3 bytes from the 4 chunks of 6 bits
		uint32_t original_triplet = decoded_values[0];
		original_triplet <<= 6;
		original_triplet |= decoded_values[1];
		original_triplet <<= 6;
		original_triplet |= decoded_values[2];
		original_triplet <<= 6;
		original_triplet |= decoded_values[3];
		// Now we can tear the uint32_t into bytes
		dest[2] = original_triplet & 0xFF;
		original_triplet >>= 8;
		dest[1] = original_triplet & 0xFF;
		original_triplet >>= 8;
		dest[0] = original_triplet & 0xFF;
		dest += 3;
	}
	return true;
}
