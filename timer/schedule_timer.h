#pragma once

#include "timer_data.h"

namespace terra
{
	//min heap, not thread-safe
    class ScheduleTimer
    {
    private:
		/** Heap of actively running timers. */
		std::vector<TimerData> active_timer_heap_;
		/** Unordered list of paused timers. */
		std::vector<TimerData> paused_timer_list_;
		/** List of timers added this frame, to be added after timer has been ticked */
		std::vector<TimerData> pending_timer_list_;

		/** An internally consistent clock, independent of World.  Advances during ticking. */
		int64_t internal_time_{ 0 };

		/** Timer delegate currently being executed.  Used to handle "timer delegates that manipulate timers" cases. */
		TimerData currently_executing_timer_;

		/** Set this to GFrameCounter when Timer is ticked. To figure out if Timer has been already ticked or not this frame. */
		int64_t last_ticked_frame_{ static_cast<int64_t>(-1) };

		/** The last handle we assigned from this timer manager */
		static int64_t last_assigned_handle_;
		
	public:
		ScheduleTimer();
		virtual ~ScheduleTimer();

		void Tick(int tick_ms);

		void ClearAllTimers();

		void SetTimer(TimerHandle& in_out_handle, const TimerCallback& timer_cb, int rate_ms, bool loop, int first_delay_ms = -1);
		void SetTimer(TimerHandle& in_out_handle, TimerCallback&& timer_cb, int rate_ms, bool loop, int first_delay_ms = -1);

		void SetTimerForNextTick(const TimerCallback& timer_cb);

		void ClearTimer(TimerHandle& in_handle)
		{
			InternalClearTimer(in_handle);
			in_handle.Invalidate();
		}

		void PauseTimer(TimerHandle timer_handle)
		{
			int timer_idx;
			TimerData const* timer_to_pause = FindTimer(timer_handle, &timer_idx);
			InternalPauseTimer(timer_to_pause, timer_idx);
		}
		void ContiuneTimer(TimerHandle timer_handle)
		{
			int timer_idx = FindTimerInList(paused_timer_list_, timer_handle);
			InternalContinueTimer(timer_idx);
		}

		int GetTimerRateMs(TimerHandle timer_handle) const
		{
			const TimerData* const timer_data = FindTimer(timer_handle);
			return InternalGetTimerRate(timer_data);
		}
		bool IsTimerActive(TimerHandle timer_handle) const
		{
			TimerData const* const timer_data = FindTimer(timer_handle);
			return timer_data && timer_data->status != ETimerStatus::PAUSED;
		}
		bool IsTimerPaused(TimerHandle timer_handle) const
		{
			TimerData const* const timer_data = FindTimer(timer_handle);
			return timer_data && timer_data->status == ETimerStatus::PAUSED;
		}
		bool IsTimerPending(TimerHandle timer_handle) const
		{
			TimerData const* const timer_data = FindTimer(timer_handle);
			return timer_data && timer_data->status == ETimerStatus::PENDING;
		}
		bool IsTimerExists(TimerHandle timer_handle) const
		{
			return FindTimer(timer_handle) != nullptr;
		}

		int64_t GetTimerRemaining(TimerHandle timer_handle) const
		{
			const TimerData* const timer_data = FindTimer(timer_handle);
			return InternalGetTimerRemaining(timer_data);
		}
		int64_t GetTimerElapsed(TimerHandle timer_handle) const
		{
			const TimerData* const timer_data = FindTimer(timer_handle);
			return InternalGetTimerElapsed(timer_data);
		}
		bool HasBeenTickedThisFrame() const { return last_ticked_frame_ == GTLFrameCounter; }

		/** Debug command to output info on all timers currently set to the log. */
		void ListTimers() const;

		/** Get the current last assigned handle */
		static void ValidateHandle(TimerHandle& in_out_handle);

	private:
		void InternalSetTimer(TimerData& new_timer, int rate_ms, bool loop, int first_delay_ms);
		void InternalClearTimer(TimerHandle& in_handle);
		void InternalClearTimer(int timer_idx, ETimerStatus timer_status);
		int64_t InternalGetTimerRemaining(const TimerData* const timer_data) const;
		int64_t InternalGetTimerElapsed(const TimerData* const timer_data) const;
		int InternalGetTimerRate(const TimerData* const timer_data) const;
		void InternalPauseTimer(TimerData const* timer_to_pause, int timer_idx);
		void InternalContinueTimer(int paused_timer_idx);

		const TimerData* FindTimer(const TimerHandle& timer_handle, int* out_timer_index = nullptr) const;
		int FindTimerInList(const std::vector<TimerData>& search_list, const TimerHandle& timer_handle) const;
    };
}