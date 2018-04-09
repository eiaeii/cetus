#pragma once

#include <string>
#include <cstdlib>

namespace terra
{
	class DebugUtils
	{
	public:
		static std::string GetCommandLineArg(int idx)
		{
			if (idx < __argc)
			{
				return std::string(__argv[idx]);
			}

			return std::string();
		}
	};
}