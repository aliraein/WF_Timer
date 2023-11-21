#ifndef TIMER_H
#define TIMER_H

#include "Worker.h"
#include <thread>
#include <atomic>

class Timer
{
public:
	Timer(Worker& worker);
	void start();
	void stop();

private:
	Worker& worker;
	std::atomic<bool> running;
	std::thread timer_thread;

	void timerLoop();
	Event generateEvent();

	std::chrono::steady_clock::time_point start_time;
};

#endif // !TIMER_H