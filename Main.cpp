#include "Timer.h"
#include "Worker.h"
#include "Job.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    //create worker instance
    Worker worker;

    //define a job --> prints event details
    Job printEventJob([](const Event& event) {
        auto now = std::chrono::steady_clock::now();
        auto execTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            event.getExecutionTime().time_since_epoch()).count();
        auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();
        std::cout << "Executing event " << event.getId()
            << " scheduled at " << execTime << "ms"
            << ", current time: " << currentTime << "ms" << std::endl;
        });

    //register job with the worker
    worker.registerJob(printEventJob);

    std::thread workerThread(&Worker::run, &worker);

    //create a timer and associate it with the worker
    Timer timer(worker);

    //start the timer to generate events
    timer.start();

    //let the system run for a short period
    std::this_thread::sleep_for(std::chrono::seconds(1));

    //stop the timer and finish
    timer.stop();
    worker.stop();
    if (workerThread.joinable()) {
        workerThread.join();
    }
    return 0;
}