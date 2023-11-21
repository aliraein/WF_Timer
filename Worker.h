//responsible for managing & executing jobs
#ifndef WORKER_H
#define WORKER_H

#include "Event.h"
#include "Job.h"
#include <queue>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <functional>

class Worker
{
public:
	Worker();
	void enqueueEvent(const Event& event);
	void registerJob(const Job& job);
	void deregisterJob(const Job& job);
	void run();
	void stop();

private:
	struct EventCompare
	{
		bool operator()(const Event& lhs, const Event& rhs) const
		{
			return lhs.getExecutionTime() > rhs.getExecutionTime();
		}
	};

	std::priority_queue<Event, std::vector<Event>, EventCompare> event_queue;
	std::vector<Job> jobs;
	std::atomic<bool> running;
	std::mutex queue_mutex;
	std::condition_variable cv;

	void executeJobs(const Event& event);
};
#endif // !WORKER_H
