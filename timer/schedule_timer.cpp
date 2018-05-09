#include "schedule_timer.h"
#include "util/vector_util.h"
using namespace terra;

void ScheduleTimer::ListTimers() const
{
	LOGF(INFO, "------- %lu Active Timers -------", active_timer_heap_.size());
	for (const auto& e : active_timer_heap_)
	{
		LOGF(INFO, "%s", e.timer_cb.target_type().name());
	}

	LOGF(INFO, "------- %lu Paused Timers -------", paused_timer_list_.size());
	for (const auto& e : paused_timer_list_)
	{
		LOGF(INFO, "%s", e.timer_cb.target_type().name());
	}
	
	LOGF(INFO, "------- %lu Pending Timers -------", pending_timer_list_.size());
	for (const auto& e : pending_timer_list_)
	{
		LOGF(INFO, "%s", e.timer_cb.target_type().name());
	}

	LOGF(INFO, "------- %lu Total Timers -------", active_timer_heap_.size() + paused_timer_list_.size() + pending_timer_list_.size());
}

int64_t ScheduleTimer::last_assigned_handle_ = 0;

//	static function
void ScheduleTimer::ValidateHandle(TimerHandle & in_out_handle)
{
	if (!in_out_handle.IsValid())
	{
		++last_assigned_handle_;
		in_out_handle.timer_handle = last_assigned_handle_;
	}
}

ScheduleTimer::ScheduleTimer()
{
}

ScheduleTimer::~ScheduleTimer()
{
}

void ScheduleTimer::SetTimer(TimerHandle & in_out_handle, const TimerCallback & timer_cb, int rate_ms, bool loop, int first_delay_ms/* = -1*/)
{
	if (in_out_handle.IsValid())
	{
		ClearTimer(in_out_handle);
	}

	if (rate_ms > 0)
	{
		ValidateHandle(in_out_handle);

		TimerData new_timer;
		new_timer.timer_handle = in_out_handle;
		new_timer.timer_cb = timer_cb;
		InternalSetTimer(new_timer, rate_ms, loop, first_delay_ms);
	}
}

void ScheduleTimer::SetTimer(TimerHandle & in_out_handle, TimerCallback&& timer_cb, int rate_ms, bool loop, int first_delay_ms/* = -1*/)
{
	if (in_out_handle.IsValid())
	{
		ClearTimer(in_out_handle);
	}

	if (rate_ms > 0)
	{
		ValidateHandle(in_out_handle);

		TimerData new_timer;
		new_timer.timer_handle = in_out_handle;
		new_timer.timer_cb = std::move(timer_cb);
		InternalSetTimer(new_timer, rate_ms, loop, first_delay_ms);
	}
}

void ScheduleTimer::SetTimerForNextTick(const TimerCallback & timer_cb)
{
	TimerData new_timer;
	new_timer.rate_ms = 0;
	new_timer.loop = false;
	new_timer.is_require_cb = true;
	new_timer.timer_cb = timer_cb;
	new_timer.expire_time = internal_time_;
	new_timer.status = ETimerStatus::ACTIVE;

	VectorUtils<decltype(active_timer_heap_)>::HeapPush(active_timer_heap_, new_timer);
}

void ScheduleTimer::InternalSetTimer(TimerData & new_timer, int rate_ms, bool loop, int first_delay_ms)
{
	if (new_timer.timer_handle.IsValid() || new_timer.timer_cb)
	{
		new_timer.rate_ms = rate_ms;
		new_timer.loop = loop;
		new_timer.is_require_cb = new_timer.timer_cb ? true : false;

		first_delay_ms = first_delay_ms > 0 ? first_delay_ms : rate_ms;
		if (HasBeenTickedThisFrame())
		{
			new_timer.expire_time = internal_time_ + first_delay_ms;
			new_timer.status = ETimerStatus::ACTIVE;
			VectorUtils<decltype(active_timer_heap_)>::HeapPush(active_timer_heap_, new_timer);
		}
		else
		{
			new_timer.expire_time = first_delay_ms;
			new_timer.status = ETimerStatus::PENDING;
			pending_timer_list_.push_back(new_timer);
		}
	}
}

const TimerData* ScheduleTimer::FindTimer(const TimerHandle& timer_handle, int* out_timer_index) const
{
	if (!timer_handle.IsValid())
	{
		return nullptr;
	}
	if (currently_executing_timer_.timer_handle == timer_handle)
	{
		if (out_timer_index)
		{
			*out_timer_index = -1;
		}
		return &currently_executing_timer_;
	}
	int active_timer_idx = FindTimerInList(active_timer_heap_, timer_handle);
	if (active_timer_idx != INDEX_NONE)
	{
		if (out_timer_index)
		{
			*out_timer_index = active_timer_idx;
		}
		return &active_timer_heap_[active_timer_idx];
	}

	int paused_timer_idx = FindTimerInList(paused_timer_list_, timer_handle);
	if (paused_timer_idx != INDEX_NONE)
	{
		if (out_timer_index)
		{
			*out_timer_index = paused_timer_idx;
		}
		return &paused_timer_list_[paused_timer_idx];
	}

	int pending_timer_idx = FindTimerInList(pending_timer_list_, timer_handle);
	if (pending_timer_idx != INDEX_NONE)
	{
		if (out_timer_index)
		{
			*out_timer_index = pending_timer_idx;
		}
		return &pending_timer_list_[pending_timer_idx];
	}

	return nullptr;
}

int ScheduleTimer::FindTimerInList(const std::vector<TimerData>& search_list, const TimerHandle& timer_handle) const
{
	if (timer_handle.IsValid())
	{
		for (size_t i = 0; i < search_list.size(); i++)
		{
			if (search_list[i].timer_handle == timer_handle)
			{
				return static_cast<int>(i);
			}
		}
	}
	return INDEX_NONE;
}

void ScheduleTimer::InternalClearTimer(TimerHandle& in_handle)
{
	//SCOPE_CYCLE_COUNTER(STAT_ClearTimer);
	if (!in_handle.IsValid())
	{
		return;
	}
	int timer_idx;
	const TimerData* const timer_data = FindTimer(in_handle, &timer_idx);
	if (timer_data)
	{
		InternalClearTimer(timer_idx, timer_data->status);
	}

}

void ScheduleTimer::InternalClearTimer(int timer_idx, ETimerStatus timer_status)
{
	switch (timer_status)
	{
	case ETimerStatus::PENDING:
		VectorUtils<decltype(pending_timer_list_)>::RemoveAtSwap(pending_timer_list_, timer_idx);
		break;

	case ETimerStatus::ACTIVE:
		VectorUtils<decltype(active_timer_heap_)>::HeapRemoveAt(active_timer_heap_, timer_idx);
		break;

	case ETimerStatus::PAUSED:
		VectorUtils<decltype(pending_timer_list_)>::RemoveAtSwap(pending_timer_list_, timer_idx);
		break;

	case ETimerStatus::EXECUTING:
		// Edge case. We're currently handling this timer when it got cleared.  Clear it to prevent it firing again
		// in case it was scheduled to fire multiple times.
		currently_executing_timer_.Clear();
		break;

	default:
		Expects(false);
	}
}

int64_t ScheduleTimer::InternalGetTimerRemaining(const TimerData* const timer_data) const
{
	if (timer_data)
	{
		switch (timer_data->status)
		{
		case ETimerStatus::ACTIVE:
			return timer_data->expire_time - internal_time_;
		case ETimerStatus::EXECUTING:
			return 0;
		default:
			// ExpireTime is time remaining for paused timers
			return timer_data->expire_time;
		}
	}
	return -1;
}

int64_t ScheduleTimer::InternalGetTimerElapsed(const TimerData* const timer_data) const
{
	if (timer_data)
	{
		switch (timer_data->status)
		{
		case ETimerStatus::ACTIVE:
		case ETimerStatus::EXECUTING:
			return timer_data->rate_ms - (timer_data->expire_time - internal_time_);
		default:
			// ExpireTime is time remaining for paused timers
			return timer_data->rate_ms - timer_data->expire_time;
		}
	}
	return -1;
}

int ScheduleTimer::InternalGetTimerRate(const TimerData* const timer_data) const
{
	if (timer_data)
	{
		return timer_data->rate_ms;
	}
	return -1;
}

void ScheduleTimer::InternalPauseTimer(TimerData const* timer_to_pause, int timer_idx)
{
	if (timer_to_pause && (timer_to_pause->status != ETimerStatus::PAUSED))
	{
		ETimerStatus previous_status = timer_to_pause->status;

		if (previous_status != ETimerStatus::EXECUTING || timer_to_pause->loop)
		{
			 paused_timer_list_.push_back(*timer_to_pause);
			 TimerData& new_timer = paused_timer_list_.back();
			 new_timer.status = ETimerStatus::PAUSED;

			 if (previous_status != ETimerStatus::PENDING)
			 {
				 new_timer.expire_time = new_timer.expire_time - internal_time_;
			 }
		}

		// Remove from previous TArray
		switch (previous_status)
		{
		case ETimerStatus::ACTIVE:
			VectorUtils<decltype(active_timer_heap_)>::HeapRemoveAt(active_timer_heap_, timer_idx);
			break;

		case ETimerStatus::PENDING:
			VectorUtils<decltype(pending_timer_list_)>::RemoveAtSwap(pending_timer_list_, timer_idx);
			break;

		case ETimerStatus::EXECUTING:
			currently_executing_timer_.Clear();
			break;

		default:
			Expects(false);
		}
	}
}

void ScheduleTimer::InternalContinueTimer(int paused_timer_idx)
{
	if (paused_timer_idx != INDEX_NONE)
	{
		TimerData& timer_to_continue = pending_timer_list_[paused_timer_idx];
		Expects(timer_to_continue.status == ETimerStatus::PAUSED);

		if (HasBeenTickedThisFrame())
		{
			timer_to_continue.expire_time += internal_time_;
			timer_to_continue.status = ETimerStatus::ACTIVE;
			VectorUtils<decltype(active_timer_heap_)>::HeapPush(active_timer_heap_, timer_to_continue);
		}
		else
		{
			timer_to_continue.status = ETimerStatus::PENDING;
			pending_timer_list_.push_back(timer_to_continue);
		}

		VectorUtils<decltype(pending_timer_list_)>::RemoveAtSwap(pending_timer_list_, paused_timer_idx);
	}
}


void ScheduleTimer::Tick(int tick_ms)
{
	if (HasBeenTickedThisFrame())
	{
		return;
	}

	internal_time_ += tick_ms;

	while (active_timer_heap_.size() > 0)
	{
		TimerData& top = VectorUtils<decltype(active_timer_heap_)>::HeadTop(active_timer_heap_);
		if (internal_time_ > top.expire_time)
		{
			// Timer has expired! Fire the delegate, then handle potential looping.

			// Remove it from the heap and store it while we're EXECUTING
			VectorUtils<decltype(active_timer_heap_)>::HeapPop(active_timer_heap_, currently_executing_timer_);
			currently_executing_timer_.status = ETimerStatus::EXECUTING;

			// Determine how many times the timer may have elapsed (e.g. for large DeltaTime on a short looping timer)
			int64_t const call_count = currently_executing_timer_.loop ?
				(internal_time_ - currently_executing_timer_.expire_time) / currently_executing_timer_.rate_ms + 1
				: 1;

			// Now call the function
			for (int64_t call_idx = 0; call_idx < call_count; ++call_idx)
			{
				if (currently_executing_timer_.timer_cb)
				{
					currently_executing_timer_.timer_cb();
				}

				// If timer was cleared in the delegate execution, don't execute further 
				if (currently_executing_timer_.status != ETimerStatus::EXECUTING)
				{
					break;
				}
			}

			// Status test needed to ensure it didn't get cleared during execution
			if (currently_executing_timer_.loop && currently_executing_timer_.status == ETimerStatus::EXECUTING)
			{
				// if timer requires a delegate, make sure it's still validly bound (i.e. the delegate's object didn't get deleted or something)
				if (!currently_executing_timer_.is_require_cb || currently_executing_timer_.timer_cb)
				{
					// Put this timer back on the heap
					currently_executing_timer_.expire_time += call_count * currently_executing_timer_.rate_ms;
					currently_executing_timer_.status = ETimerStatus::ACTIVE;
					VectorUtils<decltype(active_timer_heap_)>::HeapPush(active_timer_heap_, currently_executing_timer_);
				}
			}

			currently_executing_timer_.Clear();
		}
		else
		{
			// no need to go further down the heap, we can be finished
			break;
		}
	}

	// Timer has been ticked.
	last_ticked_frame_ = GTLFrameCounter;

	// If we have any Pending Timers, add them to the Active Queue.
	for (auto&& e : pending_timer_list_)
	{
		e.expire_time += internal_time_;
		e.status = ETimerStatus::ACTIVE;
		VectorUtils<decltype(active_timer_heap_)>::HeapPush(active_timer_heap_, e);
	}
	pending_timer_list_.clear();
}

void ScheduleTimer::ClearAllTimers()
{
	active_timer_heap_.clear();
	paused_timer_list_.clear();
	pending_timer_list_.clear();
	currently_executing_timer_.Clear();
}
