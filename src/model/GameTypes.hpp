#pragma once

#include "model/GameConfig.hpp"

namespace snakearcade::model
{
struct GridPosition
{
    int x;
    int y;
};

constexpr bool operator==(GridPosition left, GridPosition right) noexcept
{
    return left.x == right.x && left.y == right.y;
}

constexpr bool operator!=(GridPosition left, GridPosition right) noexcept
{
    return !(left == right);
}

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

enum class Collision
{
    None,
    Wall,
    Self
};

// High-level application states
enum class GameState
{
    Start,
    Playing,
    Paused,
    GameOver
};

// Difficulty levels
enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

constexpr bool isWithinBoard(GridPosition position) noexcept
{
    return position.x >= 0 && position.x < config::BoardColumns &&
           position.y >= 0 && position.y < config::BoardRows;
}
} // namespace snakearcade::model
