#pragma once

#include "model/GameTypes.hpp"
#include "model/Snake.hpp"
#include "GameTypes.hpp"

#include <vector>

namespace snakearcade::model
{
class CollisionManager final
{
public:
    // Wall collision checks
    Collision checkWallCollision(GridPosition nextHead) const noexcept;

    // Self collision checks
    // If tailMoves is true, the tail cell will be vacated upon moving forward.
    Collision checkSelfCollision(const std::vector<GridPosition>& segments, bool tailMoves = true) const noexcept;
    Collision checkSelfCollision(GridPosition nextHead, const std::vector<GridPosition>& currentSegments, bool tailMoves = true) const noexcept;

    // Combined collision checks
    Collision checkCollision(GridPosition nextHead, const std::vector<GridPosition>& currentSegments, bool tailMoves = true) const noexcept;
    Collision checkCollision(const Snake& snake, bool tailMoves = true) const noexcept;
};
} // namespace snakearcade::model

