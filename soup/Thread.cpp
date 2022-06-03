#include "Thread.hpp"

namespace soup
{
	struct CaptureThreadCreate
	{
		void(*f)(Capture&&);
		Capture cap;
	};

	static void
#if SOUP_WINDOWS
		__stdcall
#endif
		threadCreateCallback(void* handover)
	{
		auto t = reinterpret_cast<Thread*>(handover);
		auto& cap = t->create_capture.get<CaptureThreadCreate>();
		cap.f(std::move(cap.cap));
#if !SOUP_WINDOWS
		t->running = false;
#endif
		t->create_capture.reset();
	}

	Thread::Thread(void(*f)(Capture&&), Capture&& cap) noexcept
	{
		create_capture = CaptureThreadCreate{
			f,
			std::move(cap)
		};
#if SOUP_WINDOWS
		handle = CreateThread(nullptr, 0, reinterpret_cast<DWORD(__stdcall*)(LPVOID)>(&threadCreateCallback), this, 0, nullptr);
#else
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&handle, &attr, reinterpret_cast<void* (*)(void*)>(&threadCreateCallback), this);
#endif
	}

	Thread::Thread(std::function<void()>&& func) noexcept
		: Thread([](Capture&& cap)
		{
			cap.get<std::function<void()>>()();
		}, std::move(func))
	{
	}

	Thread::~Thread() noexcept
	{
#if SOUP_WINDOWS
		TerminateThread(handle, 0);
		CloseHandle(handle);
#else
		if (!joined)
		{
			pthread_cancel(handle);
			pthread_join(handle, nullptr);
		}
#endif
	}

	bool Thread::isRunning() const noexcept
	{
#if SOUP_WINDOWS
		DWORD exit_code;
		return GetExitCodeThread(handle, &exit_code)
			&& exit_code == STILL_ACTIVE
			;
#else
		return running;
#endif
	}

	void Thread::stop() noexcept
	{
#if SOUP_WINDOWS
		TerminateThread(handle, 0);
#else
		if (!joined)
		{
			pthread_cancel(handle);
			pthread_join(handle, nullptr);
			joined = true;
		}
#endif
	}

	void Thread::awaitCompletion() noexcept
	{
#if SOUP_WINDOWS
		WaitForSingleObject(handle, INFINITE);
#else
		if (!joined)
		{
			pthread_join(handle, nullptr);
			joined = true;
		}
#endif
	}

	void Thread::awaitCompletion(std::vector<UniquePtr<Thread>>& threads) noexcept
	{
#if SOUP_WINDOWS
		std::vector<HANDLE> handles{};
		for (auto& t : threads)
		{
			handles.emplace_back(t->handle);
		}
		WaitForMultipleObjects((DWORD)handles.size(), handles.data(), TRUE, INFINITE);
#else
		for (auto& t : threads)
		{
			t->awaitCompletion();
		}
#endif
	}
}
