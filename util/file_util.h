#pragma once
#include "core.h"
#include "util/string_util.h"
#ifdef __linux__
#include <sys/stat.h>
#include <unistd.h>
#define _fileno fileno
#endif

namespace terra
{
	using BufferPtr = std::unique_ptr<char, std::default_delete<char[]>>;
	class FileUtils
	{
	public:
		static BufferPtr ReadFile(const std::string& path)
		{
			FILE* fp = std::fopen(path.c_str(), "rb");
			Expects(fp);
			int descriptor = _fileno(fp);
			struct stat statBuf;
			if (fstat(descriptor, &statBuf) == -1) {
				fclose(fp);
				return nullptr;
			}
			std::size_t file_length = statBuf.st_size;
			//char* buffer = new char[file_length + 1]();  //+'\0'
			BufferPtr buffer(new char[file_length + 1]);//+'\0'
			std::size_t readsize = fread(buffer.get(), 1, file_length, fp);
			Ensures(readsize == file_length);
			fclose(fp);
			return std::move(buffer);
		}
		static std::string GetFileName(const std::string& path)
		{
			//TODO:  be care of'\' '/'
			return "";
		}

		static std::string GetFileNameWithoutExtension(const std::string& path)
		{
			//
			return "";
		}

		static std::string GetFileExtensionName(const std::string& file_name)
		{
			size_t pos = file_name.find_last_of('.');
			if (pos != std::string::npos)
			{
				return file_name.substr(pos + 1);
			}
			return std::string();
		}
	};
}