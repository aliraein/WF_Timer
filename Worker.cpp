#include "Worker.h"
#include <iostream>

Worker::Worker() : running(true) {}

void Worker::enqueueEvent(const Event& event)
{
	std::lock_guard<std::mutex> lock(queue_mutex);
	event_queue.push(event);
	cv.notify_one();
}

void Worker::registerJob(const Job& job)
{
	std::lock_guard<std::mutex> lock(queue_mutex);
	jobs.push_back(job);
}

void Worker::deregisterJob(const Job& job)
{
	std::lock_guard<std::mutex> lock(queue_mutex);
	const auto& it = std::find(jobs.begin(), jobs.end(), job);
	if (it != jobs.end())
	{
		jobs.erase(it);
	}
}

void Worker::executeJobs(const Event& event) {
	auto now = std::chrono::steady_clock::now();
	auto nowMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	for (const auto& job : jobs) {
		std::cout << "Executing job for event " << event.getId()
			<< " at current time: " << nowMillis << "ms\n";
		job.execute(event);
	}
}



void Worker::run()
{
	while (running)
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		cv.wait(lock, [this] {return !event_queue.empty() || !running; });

		if (!running && event_queue.empty()) {
			break;
		}

		const auto& event = event_queue.top();
		auto now = std::chrono::steady_clock::now();
		if (event.getExecutionTime() > now) {
			cv.wait_until(lock, event.getExecutionTime());
			//now = std::chrono::steady_clock::now();
		}

		if (event_queue.empty() || event_queue.top().getExecutionTime() != event.getExecutionTime()) {
			continue;
		}

		std::cout << "Dequuing event " << event.getId() << std::endl;
		event_queue.pop();
		lock.unlock();

		executeJobs(event);
	}
}

void Worker::stop() {
	running = false;
	cv.notify_all();
}