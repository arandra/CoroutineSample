#pragma once

#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
#include <experimental/resumable>
#endif // _RESUMABLE_FUNCTIONS_SUPPORTED

namespace custom
{
	struct resumable
	{
		struct promise_type;
		using handle = std::experimental::coroutine_handle<promise_type>;
		struct promise_type
		{
			static auto get_return_object_on_allocation_failure() { return resumable{ nullptr }; }
			auto get_return_object() { return resumable{ handle::from_promise(*this) }; }
			auto initial_suspend() { return std::experimental::suspend_never{}; }
			auto final_suspend() { return std::experimental::suspend_never{}; }
			void unhandled_exception() { std::terminate(); }
			void return_void() {}
		};
	private:
		handle _coroutine = nullptr;
	public:
		resumable() = default;
		resumable(resumable const&) = delete;
		resumable(resumable&& other) noexcept : _coroutine(other._coroutine)
		{
			other._coroutine = nullptr;
		}

		explicit resumable(handle coroutine) : _coroutine(coroutine) 
		{
		}

		resumable& operator = (resumable const&) = delete;
		resumable& operator = (resumable&& other) noexcept
		{
			if (&other != this)
			{
				_coroutine = other._coroutine;
				other._coroutine = nullptr;
			}
			return *this;
		}

		void resume()
		{
			_coroutine.resume();
		}
	};
}
