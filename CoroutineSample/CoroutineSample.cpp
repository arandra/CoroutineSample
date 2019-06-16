// CoroutineSample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <future>
#include <thread>
#include "custom\resumable.h"
#include "custom\coroutine_manager.h"

int GetInputNumber()
{
	std::cout << "B: GetInputNumber in thread id " << std::this_thread::get_id() << "\n";

	std::cout << " - Input number and press [enter] : ";
	int value = 0;
	std::cin >> value;
	return value;
}

std::future<void> WaitInputNumber(int& returnValue)
{
	std::cout << "A: Start WaitInputNumber in thread id " << std::this_thread::get_id() << "\n";
	auto value = co_await std::async(std::launch::async, GetInputNumber);
	std::cout << "C: Resume WaitInputNumber in thread id " << std::this_thread::get_id() << "\n";
	returnValue = value;
}

static custom::coroutine_manager CoroutineManager;

custom::resumable WaitInputNumberWithCustomResumable(int& returnValue)
{
	std::cout << "A: Start WaitInputNumber in thread id " << std::this_thread::get_id() << "\n";
	auto value = CoroutineManager.AwaitCoroutine<int>(GetInputNumber);
	co_await std::experimental::suspend_always{};
	std::cout << "C: Resume WaitInputNumber in thread id " << std::this_thread::get_id() << "\n";
	returnValue = value.get();
}

int main()
{
	int inputValue = 0;
	//--------------------- sample with future ------------------------
	std::cout << "--------------------- sample with future ------------------------\n";
	auto resumable = WaitInputNumber(inputValue);
	while (!resumable._Is_ready())
	{
		continue;
	}
	std::cout << " - Input number is "<< inputValue <<". -\n";
    std::cout << "D: Finished in thread id " << std::this_thread::get_id() << "\n";

	//--------------------- sample with custom resumable --------------------
	std::cout << "\n--------------------- sample with custom resumable ------------------------\n";
	CoroutineManager.StartCoroutine(WaitInputNumberWithCustomResumable(inputValue));
	while (CoroutineManager.Update())
	{
		continue;
	}
	std::cout << " - Input number is " << inputValue << ". -\n";
	std::cout << "D: Finished in thread id " << std::this_thread::get_id() << "\n";
}

