#pragma once
#ifndef TIMER_H
#include <chrono>
class Timer {
public:
    int ticks;
    float partialTicks;

    Timer(int ticksPerSecond);

    void advanceTime();

private:
    int ticksPerSecond;
    std::chrono::steady_clock::time_point lastTime;
    double tickAccumulator;
};
#endif // TIMER_H