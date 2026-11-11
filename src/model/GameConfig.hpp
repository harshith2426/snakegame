#pragma once

namespace snakearcade::config
{
inline constexpr int BoardColumns = 40;
inline constexpr int BoardRows = 30;
inline constexpr int CellSizePixels = 20;
inline constexpr int InitialSnakeLength = 3;
inline constexpr int PointsPerFood = 10;

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

inline constexpr float EasyMovementInterval = 0.20F;
inline constexpr float MediumMovementInterval = 0.15F;
inline constexpr float HardMovementInterval = 0.08F;

constexpr float movementInterval(Difficulty difficulty) noexcept
{
    switch (difficulty)
    {
    case Difficulty::Easy:
        return EasyMovementInterval;
    case Difficulty::Medium:
        return MediumMovementInterval;
    case Difficulty::Hard:
        return HardMovementInterval;
    }

    return MediumMovementInterval;
}
} // namespace snakearcade::config
