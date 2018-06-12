#pragma once

#include "timer_handle.h"

namespace terra
{
	enum class ETimerStatus : uint8_t
	{
		PENDING,
		ACTIVE,
		PAUSED,
		EXECUTING,
	};

	using TimerCallback = std::function<void()>;

	struct TimerDataLite
	{
		TimerDataLite(int64_t in_expire_time, const TimerCallback& cb)
			: expire_time(in_expire_time)
			, timer_cb(cb)
		{
		}
		TimerDataLite(int64_t in_expire_time, TimerCallback&& cb)
			: expire_time(in_expire_time)
			, timer_cb(std::move(cb))
		{
		}
		int64_t expire_time;
		TimerCallback timer_cb;
		bool operator<(const TimerDataLite& rhs) const
		{
			return expire_time < rhs.expire_time;
		}
		bool operator>(const TimerDataLite& rhs) const
		{
			return expire_time > rhs.expire_time;
		}
	};


	struct TimerData
	{
		bool loop{ false };
		/** If true, this timer was created with a delegate to call (which means if the delegate becomes invalid, we should invalidate the timer too). */
		bool is_require_cb{ false };
		ETimerStatus status{ ETimerStatus::ACTIVE };
		int rate_ms{ 0 };
		int64_t expire_time{ 0 };
		TimerCallback timer_cb;
		TimerHandle timer_handle;
		bool operator<(const TimerData& rhs) const
		{
			return expire_time < rhs.expire_time;
		}

		void Clear()
		{
			timer_cb = nullptr;
			timer_handle.Invalidate();
		}
	};
}