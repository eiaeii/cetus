#include "runnable_thread.h"

using namespace terra;

RunnableThread* RunnableThread::CreateThread(Runnable* runnable, const char* thread_name)
{
	RunnableThread* new_thread = new RunnableThread();
	if (new_thread)
	{
		if (!new_thread->CreateInternal(runnable, thread_name))
		{
			delete new_thread;
			new_thread = nullptr;
		}
	}
	return new_thread;
}

bool RunnableThread::CreateInternal(Runnable* runnable, const char* thread_name)
{
	runnable_ = runnable;
	thread_name_ = thread_name;
	t_ = new std::thread([this]() {
		//FThreadManager::Get().AddThread(ThisThread->ThreadID, ThisThread);
		this->PreRun();
		this->Run();
		this->PostRun();
	});
	t_->detach();
	return true;
}
