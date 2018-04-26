#pragma once

#include "core.h"
#include <cstdio>
namespace terra
{
	class StringUtils
	{
	public:
		static int Format(std::string& str, const char* fmt, ...)
		{
			va_list ap;
			va_start(ap, fmt);
			int ret = FormatV(str, fmt, ap);
			va_end(ap);
			return ret;
		}
		static std::string Format(const char* fmt, ...)
		{
			std::string str;
			va_list ap;
			va_start(ap, fmt);
			FormatV(str, fmt, ap);
			va_end(ap);
			return str;
		}
		static int Replace(std::string& str, char oldch, char newch)
		{
			if (oldch == newch)
			{
				return 0;
			}

			int count = 0;
			for (size_t i = 0; i < str.length(); ++i)
			{
				if (str[i] == oldch)
				{
					str[i] = newch;
					++count;
				}
			}

			return count;
		}

		static int Replace(std::string& str, const char* oldchs, const char* newchs)
		{
			if (oldchs == newchs)
			{
				return 0;
			}

			const size_t oldlen = strlen(oldchs);
			const size_t newlen = strlen(newchs);
			int count = 0;
			size_t index = str.find(oldchs, 0);
			while (index != std::string::npos)
			{
				str.erase(index, oldlen);
				str.insert(index, newchs);
				index = str.find(oldchs, index + newlen);
				++count;
			}

			return count;
		}

		static bool IsEqual(const std::string& lhs, const std::string& rhs, bool ignore_case = false)
		{
#ifdef _WIN32
			return ignore_case ? (_stricmp(lhs.c_str(), rhs.c_str()) == 0) : (strcmp(lhs.c_str(), rhs.c_str()) == 0);
#else
			return ignore_case ? (strcasecmp(lhs.c_str(), rhs.c_str()) == 0) : (strcmp(lhs.c_str(), rhs.c_str()) == 0);
#endif
		}
		static bool IsEqual(const char* lhs, const char* rhs, bool ignore_case = false)
		{
#ifdef _WIN32
			return ignore_case ? (_stricmp(lhs, rhs) == 0) : (strcmp(lhs, rhs) == 0);
#else
			return ignore_case ? (strcasecmp(lhs, rhs) == 0) : (strcmp(lhs, rhs) == 0);
#endif
		}
		static bool StartsWith(const std::string& str, const char* prefix)
		{
			return prefix && *prefix && !strncmp(str.c_str(), prefix, strlen(prefix));
		}
		static bool StartsWith(const std::string& str, const std::string& prefix) 
		{
			return prefix.length() > 0 && !strncmp(str.c_str(), prefix.c_str(), prefix.length());
		}
		static bool EndsWith(const std::string& str, const char* suffix)
		{
			if (!suffix || *suffix == '\0')
			{
				return false;
			}

			size_t string_len = str.length();
			size_t suffix_len = strlen(suffix);
			if (suffix_len > string_len)
			{
				return false;
			}

			const char* strptr = str.data() + string_len - suffix_len;
			return !strcmp(strptr, suffix);
		}
		static bool EndsWith(const std::string& str, const std::string& suffix)
		{
			return EndsWith(str, suffix.c_str());
		}
		static int Split(const std::string& str, const std::string& separator, std::vector<std::string>& strs)
		{
			size_t pos1 = 0, pos2 = 0;
			int count = 0;
			while (true)
			{
				pos1 = str.find_first_not_of(separator, pos2);
				if (pos1 == std::string::npos)
				{
					break;
				}

				pos2 = str.find_first_of(separator, pos1);
				if (pos2 == std::string::npos)
				{
					strs.push_back(str.substr(pos1));
					++count;
					break;
				}
				else
				{
					strs.push_back(str.substr(pos1, pos2 - pos1));
					++count;
				}
			}
			return count;
		}
	private:
		static int FormatV(std::string& str, const char* fmt, va_list ap)
		{
			va_list apcopy;
			va_copy(apcopy, ap);
			int size = ::vsnprintf(nullptr, 0, fmt, ap);  //caculate the memory space we need
			if (size <= 0)
			{
				return size;
			}
			++size;

			int len = 0;
			char* buf = new char[size];
			if (!buf)
			{
				return -1;
			}
			len = ::vsnprintf(buf, size, fmt, apcopy);
			va_end(apcopy);

			if (len < 0 || len >= size)
			{
				delete[] buf;
				return len;
			}

			buf[len] = 0;
			str.assign(buf, len);
			delete[] buf;

			return len;
		}
	};
}