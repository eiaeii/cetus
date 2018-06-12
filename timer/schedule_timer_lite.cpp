#include "schedule_timer_lite.h"

using namespace terra;


ScheduleTimerLite::ScheduleTimerLite(int64_t tick_ms)
	: kTickIntervalMs(tick_ms)
{
	elapsed_time_ = 0;
	next_sample_time_ = elapsed_time_ + kTickIntervalMs;
}

void ScheduleTimerLite::RunAfter(int delay_ms, const TimerCallback& timer_cb)
{
	timers_.emplace(elapsed_time_ + delay_ms, timer_cb);
}

void ScheduleTimerLite::RunAfter(int delay_ms, TimerCallback&& timer_cb)
{
	timers_.emplace(elapsed_time_ + delay_ms, std::move(timer_cb));
}

void ScheduleTimerLite::Tick(int tick_ms)
{
	elapsed_time_ += tick_ms;
	if (!IsTimeToTick())
	{
		return;
	}
	next_sample_time_ += kTickIntervalMs;
	while (!timers_.empty())
	{
		auto& e = timers_.top();
		if (elapsed_time_ >= e.expire_time)
		{
			e.timer_cb();
			timers_.pop();
		}
		else
		{
			break;
		}
	}
}

//#include <iostream>
//#include <thread>
//#include <chrono>
//
//void test()
//{
//	ScheduleTimerLite stl(1000);
//	stl.RunAfter(1000, []() { std::cout << "after 1280ms, timer called!" << std::endl; });
//	stl.RunAfter(5280, []() { std::cout << "after 5280ms, timer called!" << std::endl; });
//	stl.RunAfter(10280, []() { std::cout << "after 10280ms, timer called!" << std::endl; });
//
//	using namespace std::chrono;
//	using namespace std::literals;
//	for (int i = 0; i < 150; i++)
//	{
//		stl.Tick(100);
//		std::this_thread::sleep_for(100ms);
//	}
//
//	std::cout << "end" << std::endl;
//}
