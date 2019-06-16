#pragma once

#include "resumable.h"
#include <future>
#include <functional>
#include <vector>

namespace custom
{
	class ICoroutineItem
	{
	public:
		virtual void SetResumable(resumable&& other) = 0;
		virtual bool IsReady() = 0;
		virtual void Resume() = 0;
		virtual ~ICoroutineItem() {};
	};

	template<class T>
	class coroutine_item : public ICoroutineItem
	{
	public:
		explicit coroutine_item(std::function<T()>&& func) : _func(std::move(func))
		{
		}

		virtual ~coroutine_item() override
		{
		}

		virtual void SetResumable(resumable&& other) override
		{
			_resumable = std::move(other);
		}

		virtual bool IsReady() override
		{
			return _future._Is_ready();
		}

		virtual void Resume() override
		{
			_promise.set_value(_future.get());
			_resumable.resume();
		}

		void Run()
		{
			_future = std::async(std::launch::async, _func);
		}

		std::shared_future<T> GetSharedFuture()
		{
			std::shared_future<T> shared = _promise.get_future().share();
			return std::move(shared);
		}

	private:
		std::function<T()> _func;
		resumable _resumable;
		std::future<T> _future;
		std::promise<T> _promise;
	};

	class coroutine_manager
	{
	public:
		coroutine_manager();
		~coroutine_manager();

		void StartCoroutine(resumable&& routine);

		template<class T>
		auto AwaitCoroutine(std::function<T()>&& func) -> std::shared_future<T>
		{
			auto* item = new custom::coroutine_item<T>(std::move(func));
			item->Run();
			lastItem = item;
			return std::move(item->GetSharedFuture());
		}

		bool Update();

	private:
		std::vector<ICoroutineItem*> items;
		ICoroutineItem* lastItem = nullptr;
	};
};