#pragma once

#include <iostream>
#include <chrono>
#include <thread>

class timer
{
    // alias our types for simplicity
    using clock = std::chrono::system_clock;
    using time_point_type = std::chrono::time_point < clock, std::chrono::milliseconds >;
public:
    // default constructor that stores the start time
    timer()
    {
        startTime = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    }

    void start() {
        startTime = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
    }
    // gets the time elapsed from construction.
    long /*milliseconds*/ getTimePassed()
    {
        // get the new time
        auto end = clock::now();

        // return the difference of the times
        return (end - startTime).count();
    }

private:
    time_point_type startTime;
};