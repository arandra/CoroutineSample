#include "coroutine_manager.h"

using namespace custom;

coroutine_manager::coroutine_manager()
{
}

coroutine_manager::~coroutine_manager()
{
}

void coroutine_manager::StartCoroutine(resumable&& routine)
{
	if (lastItem != nullptr)
	{
		lastItem->SetResumable(std::move(routine));
		items.push_back(lastItem);
		lastItem = nullptr;
	}
}

bool coroutine_manager::Update()
{
	if (items.empty())
		return false;

	std::vector<ICoroutineItem*> finished;
	for (auto item : items)
	{
		if (item->IsReady())
		{
			item->Resume();
			finished.push_back(item);
		}
	}

	for (auto item : finished)
	{
		items.erase(std::remove(items.begin(), items.end(), item), std::end(items));
		delete item;
	}

	return true;
}