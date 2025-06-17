#include "Timer.h"

Timer::Timer(int ticksPerSecond)
    : ticks(0), partialTicks(0.0f), ticksPerSecond(ticksPerSecond), tickAccumulator(0.0)
{
    lastTime = std::chrono::steady_clock::now();
}

void Timer::advanceTime() {
    using namespace std::chrono;
    auto now = steady_clock::now();
    double elapsed = duration<double>(now - lastTime).count();
    lastTime = now;

    tickAccumulator += elapsed * ticksPerSecond;
    ticks = static_cast<int>(tickAccumulator);
    tickAccumulator -= ticks;
    partialTicks = static_cast<float>(tickAccumulator);
}