#pragma once

namespace snakearcade::model
{
// Counts fixed movement ticks independently of the render frame rate.
class MovementTimer final
{
public:
    explicit MovementTimer(float intervalSeconds = 0.15F) noexcept;
    int advance(float elapsedSeconds) noexcept;
    void reset() noexcept;

private:
    float intervalSeconds_;
    float accumulatedSeconds_ = 0.0F;
};
} // namespace snakearcade::model
