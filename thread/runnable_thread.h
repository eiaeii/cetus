#pragma once

#include "core.h"
#include "runnable.h"

namespace terra
{
	class RunnableThread
	{
	private:
		std::string thread_name_;
		Runnable* runnable_{ nullptr };
		std::thread* t_{ nullptr };
	public:
		~RunnableThread()
		{
			if (t_)
			{
				Kill();
				delete t_;
			}
			//if (!GIsRequestingExit)
			//{
			//	FThreadManager::Get().RemoveThread(this);
			//}
		}

		static std::unique_ptr<RunnableThread> CreateThread(Runnable* runnable, const char* thread_name);

		virtual void PreRun() { }
		virtual void PostRun() { }
		uint32_t Run()
		{
			uint32_t exit_code = 1;
			Expects(runnable_);
			if (runnable_->Init() == true)
			{
				// Initialization has completed, release the sync event
				//ThreadInitSyncEvent->Trigger();

				exit_code = runnable_->Run();
				runnable_->Exit();
			}
			else
			{
				// Initialization has failed, release the sync event
				//ThreadInitSyncEvent->Trigger();
			}
			return exit_code;
		}
		bool Kill()
		{
			if (runnable_)
			{
				runnable_->Stop();
			}
			if (t_ && t_->joinable())
			{
				t_->join();
			}
			return true;
		}

		void WaitForCompletion()
		{
			if (t_ && t_->joinable())
			{
				t_->join();
			}
		}

	protected:
		bool CreateInternal(Runnable* runnable, const char* thread_name);
	};
}