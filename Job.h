//the interface. can be defined using std::fucntion for allowing flexibility

#ifndef JOB_H
#define JOB_H

#include "Event.h"
#include <functional>

class Job
{
public:
	using JobFunction = std::function<void(const Event&)>;

	Job(int id, JobFunction func);
	int getId() const;
	void execute(const Event& event) const;
	bool operator==(const Job& other) const;

	Job(JobFunction func);

private:
	int job_id;
	JobFunction job_function;
};

#endif // !JOB_H