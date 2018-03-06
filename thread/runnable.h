#pragma once

#include "core.h"

namespace terra
{
	/**
	* Interface for ticking runnables when there's only one thread available and
	* multithreading is disabled.
	*/
	class SingleThreadRunnable
	{
	public:

		virtual ~SingleThreadRunnable() = default;

		/* Tick function. */
		virtual void Tick() = 0;
	};

	/**
	* Interface for "runnable" objects.
	*
	* A runnable object is an object that is "run" on an arbitrary thread. The call usage pattern is
	* Init(), Run(), Exit(). The thread that is going to "run" this object always uses those calling
	* semantics. It does this on the thread that is created so that any thread specific uses (TLS, etc.)
	* are available in the contexts of those calls. A "runnable" does all initialization in Init().
	*
	* If initialization fails, the thread stops execution and returns an error code. If it succeeds,
	* Run() is called where the real threaded work is done. Upon completion, Exit() is called to allow
	* correct clean up.
	*/
	class Runnable
	{
	public:
		virtual bool Init() { return true; }
		virtual uint32_t Run() = 0;
		virtual void Stop() { }
		virtual void Exit() { }

		/**
		* Gets single thread interface pointer used for ticking this runnable when multi-threading is disabled.
		* If the interface is not implemented, this runnable will not be ticked when FPlatformProcess::SupportsMultithreading() is false.
		*
		* @return Pointer to the single thread interface or nullptr if not implemented.
		*/
		virtual class SingleThreadRunnable* GetSingleThreadInterface() { return nullptr; }
		virtual ~Runnable() = default;
	};
}