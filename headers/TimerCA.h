/*
 @author - Sol Zilberman
 @header - TimerCA.h
 @description - A simple timer class do assist with performance measurement
 */
#include <chrono>
#ifndef TIMER_H
#define TIMER_H



class TimerCA {
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;   // start time
    TimerCA() {
        this->start = high_resolution_clock::now();
    }
    void _start() { // start the timer
        this->start = high_resolution_clock::now();
    }
    double _stop() { // returns time in milliseconds
        auto end = high_resolution_clock::now();
        duration<double, std::milli> ms = end - start;
        return ms.count();
    }
};

#endif // TIMER_H