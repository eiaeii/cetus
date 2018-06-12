#pragma once

#include "timer_data.h"
#include  <queue>

namespace terra
{
	//priority_queue
	// only support run after once
	class ScheduleTimerLite
	{
	private:
		std::priority_queue<TimerDataLite, std::vector<TimerDataLite>, std::greater<TimerDataLite>> timers_;

		int64_t elapsed_time_{ 0 };
		int64_t next_sample_time_{ 0 };
		const int64_t kTickIntervalMs;
	public:
		ScheduleTimerLite(int64_t tick_ms);
		void RunAfter(int delay_ms, const TimerCallback& timer_cb);
		void RunAfter(int delay_ms, TimerCallback&& timer_cb);

		void Tick(int tick_ms);

		bool IsTimeToTick() { return elapsed_time_ >= next_sample_time_; }
	};

}