#include "runnable_thread.h"

using namespace terra;

std::unique_ptr<RunnableThread> RunnableThread::CreateThread(Runnable* runnable, const char* thread_name)
{
	std::unique_ptr<RunnableThread>  new_thread = std::make_unique<RunnableThread>();
	if (new_thread)
	{
		if (!new_thread->CreateInternal(runnable, thread_name))
		{
			new_thread.reset(nullptr);
		}
	}
	return new_thread;
}

bool RunnableThread::CreateInternal(Runnable* runnable, const char* thread_name)
{
	runnable_ = runnable;
	thread_name_ = thread_name;
	t_ = std::thread([this]() {
		//FThreadManager::Get().AddThread(ThisThread->ThreadID, ThisThread);
		this->PreRun();
		this->Run();
		this->PostRun();
	});
	return true;
}
