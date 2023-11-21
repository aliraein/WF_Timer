//this includes the event ID & the execution time

#ifndef EVENT_H
#define EVENT_H

#include <chrono>

class Event
{
public:
	Event(int id, std::chrono::steady_clock::time_point execTime);

	int getId() const;
	std::chrono::steady_clock::time_point getExecutionTime() const;

private:
	int event_id;
	std::chrono::steady_clock::time_point execution_time;
};

#endif // !EVENT_H