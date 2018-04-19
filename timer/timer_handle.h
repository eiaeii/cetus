#pragma once

#include "core.h"
#include "util/string_util.h"

namespace terra
{
	class ScheduleTimer;
	struct TimerHandle
	{
	private:
		uint64_t timer_handle{ 0 };
	public:
		friend class ScheduleTimer;
		bool IsValid() const
		{
			return timer_handle != 0;
		}

		void Invalidate()
		{
			timer_handle = 0;
		}

		bool operator==(const TimerHandle& rhs) const
		{
			return timer_handle == rhs.timer_handle;
		}

		bool operator!=(const TimerHandle& rhs) const
		{
			return timer_handle != rhs.timer_handle;
		}

		std::string ToString() const
		{
			return StringUtils::Format("%ull", timer_handle);
		}

	};
}