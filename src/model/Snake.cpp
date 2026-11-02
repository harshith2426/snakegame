#include "model/Snake.hpp"

#include "model/GameConfig.hpp"

#include <algorithm>

namespace snakearcade::model
{
Snake::Snake()
    : segments_{
          {config::BoardColumns / 2, config::BoardRows / 2},
          {config::BoardColumns / 2 - 1, config::BoardRows / 2},
          {config::BoardColumns / 2 - 2, config::BoardRows / 2}},
      direction_(Direction::Right)
{
}

const std::vector<GridPosition>& Snake::segments() const noexcept
{
    return segments_;
}

GridPosition Snake::head() const noexcept
{
    return segments_.front();
}

Direction Snake::direction() const noexcept
{
    return direction_;
}

bool Snake::setDirection(Direction direction) noexcept
{
    const bool reversing =
        (direction_ == Direction::Up && direction == Direction::Down) ||
        (direction_ == Direction::Down && direction == Direction::Up) ||
        (direction_ == Direction::Left && direction == Direction::Right) ||
        (direction_ == Direction::Right && direction == Direction::Left);
    if (reversing)
    {
        return false;
    }
    direction_ = direction;
    return true;
}

void Snake::move()
{
    GridPosition nextHead = head();
    switch (direction_)
    {
    case Direction::Up: --nextHead.y; break;
    case Direction::Down: ++nextHead.y; break;
    case Direction::Left: --nextHead.x; break;
    case Direction::Right: ++nextHead.x; break;
    }
    // Keep the model on the configured grid until collision/game-over behavior
    // is implemented in its dedicated sprint task.
    if (!isWithinBoard(nextHead))
    {
        return;
    }

    for (std::size_t index = segments_.size() - 1; index > 0; --index)
    {
        segments_[index] = segments_[index - 1];
    }
    segments_.front() = nextHead;
}

std::size_t Snake::length() const noexcept
{
    return segments_.size();
}

bool Snake::occupies(GridPosition position) const noexcept
{
    return std::find(segments_.begin(), segments_.end(), position) != segments_.end();
}
} // namespace snakearcade::model
