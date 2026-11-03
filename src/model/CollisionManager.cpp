#include "model/CollisionManager.hpp"

namespace snakearcade::model
{
Collision CollisionManager::checkWallCollision(GridPosition nextHead) const noexcept
{
    if (!isWithinBoard(nextHead))
    {
        return Collision::Wall;
    }
    return Collision::None;
}

Collision CollisionManager::checkSelfCollision(const std::vector<GridPosition>& segments, bool tailMoves) const noexcept
{
    if (segments.size() <= 3)
    {
        return Collision::None;
    }

    const GridPosition head = segments.front();
    const std::size_t limit = tailMoves ? segments.size() - 1 : segments.size();

    for (std::size_t i = 1; i < limit; ++i)
    {
        if (head == segments[i])
        {
            return Collision::Self;
        }
    }

    return Collision::None;
}

Collision CollisionManager::checkSelfCollision(GridPosition nextHead,
                                               const std::vector<GridPosition>& currentSegments,
                                               bool tailMoves) const noexcept
{
    if (currentSegments.size() <= 3)
    {
        return Collision::None;
    }

    const std::size_t limit = tailMoves ? currentSegments.size() - 1 : currentSegments.size();

    for (std::size_t i = 0; i < limit; ++i)
    {
        if (nextHead == currentSegments[i])
        {
            return Collision::Self;
        }
    }

    return Collision::None;
}

Collision CollisionManager::checkCollision(GridPosition nextHead,
                                           const std::vector<GridPosition>& currentSegments,
                                           bool tailMoves) const noexcept
{
    const Collision wall = checkWallCollision(nextHead);
    if (wall != Collision::None)
    {
        return wall;
    }

    return checkSelfCollision(nextHead, currentSegments, tailMoves);
}

Collision CollisionManager::checkCollision(const Snake& snake, bool tailMoves) const noexcept
{
    return checkCollision(snake.nextHead(), snake.segments(), tailMoves);
}
} // namespace snakearcade::model

