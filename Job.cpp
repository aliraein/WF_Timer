#include "Job.h"
#include <iostream>

Job::Job(int id, JobFunction func) : job_id(id), job_function(func) {}

int Job::getId() const
{
	return job_id;
}

bool Job::operator==(const Job& other) const {
	return job_id == other.job_id;
}

Job::Job(JobFunction func) : job_function(func) {
	static int nextId = 0;
	job_id = nextId++;
}

void Job::execute(const Event& event) const {
	auto now = std::chrono::steady_clock::now();
	auto nowMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch());
	std::cout << "Executing job for event " << event.getId()
		<< " at " << nowMillis << "ms" << std::endl;

	job_function(event);
}