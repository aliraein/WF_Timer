#include "Event.h"

Event::Event(int id, std::chrono::steady_clock::time_point execTime)
	: event_id(id), execution_time(execTime) {}

int Event::getId() const
{
	return event_id;
}

std::chrono::steady_clock::time_point Event::getExecutionTime() const
{
	return execution_time;
}