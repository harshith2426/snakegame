#include "model/MovementTimer.hpp"

#include <cmath>

namespace snakearcade::model
{
MovementTimer::MovementTimer(float intervalSeconds) noexcept
    : intervalSeconds_(intervalSeconds > 0.0F && std::isfinite(intervalSeconds)
                           ? intervalSeconds
                           : 0.15F)
{
}

int MovementTimer::advance(float elapsedSeconds) noexcept
{
    if (elapsedSeconds <= 0.0F || !std::isfinite(elapsedSeconds))
    {
        return 0;
    }

    accumulatedSeconds_ += elapsedSeconds;
    int ticks = 0;
    while (accumulatedSeconds_ >= intervalSeconds_)
    {
        accumulatedSeconds_ -= intervalSeconds_;
        ++ticks;
    }
    return ticks;
}

void MovementTimer::reset() noexcept
{
    accumulatedSeconds_ = 0.0F;
}
} // namespace snakearcade::model
