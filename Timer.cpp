#include "Timer.h"
#include <iostream>
#include <random>

Timer::Timer(Worker& worker) : worker(worker), running(false) {
	start_time = std::chrono::steady_clock::now();
}

void Timer::start()
{
	running = true;
	timer_thread = std::thread(&Timer::timerLoop, this);
}

void Timer::stop()
{
	running = false;
	if (timer_thread.joinable())
	{
		timer_thread.join();
	}
}

Event Timer::generateEvent()
{
	static int event_id = 0;
	auto now = std::chrono::steady_clock::now();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 500);

	auto exec_time = now + std::chrono::milliseconds(dis(gen));

	//return Event(event_id++, exec_time);
	Event event(event_id++, exec_time);

	//print statement to debug!
	auto execTimeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
		exec_time - start_time).count();
	std::cout << "Generated event " << event.getId()
		<< " with execution time: " << execTimeMillis << "ms" << std::endl;

	return event;
}

void Timer::timerLoop() {
	while (running) {
		auto event = generateEvent();
		worker.enqueueEvent(event);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}