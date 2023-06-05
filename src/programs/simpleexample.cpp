#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <remoteexecutor/executor.hpp>
#include <remoteexecutor/work.hpp>

namespace
{
	struct Task1
	{
		uint32_t id;

		Task1():
			id(0U)
		{
		}

		void operator()()
		{
			while (id++ < 100000000U);
		}
	};

	void task2()
	{
		std::this_thread::sleep_for(std::chrono::seconds(3U));
	}
}

int main()
{
	remoteexecutor::Executor e(10U);
	uint32_t taskCounter(0U);
	bool isRunning(true);
	Task1 task;

	e.setReadyHandler([&taskCounter, &isRunning](remoteexecutor::Work&&)
		{
			if (++taskCounter == 3U)
				isRunning = false;
		});
	e.execute(task, 1U);
	e.execute(task2, 1U);
	e.execute([](){ std::cout << "Task3 done" << std::endl; }, 2U);

	while (isRunning)
		e.handleReady();

	return EXIT_SUCCESS;
}
