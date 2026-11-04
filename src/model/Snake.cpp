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

Snake::Snake(std::vector<GridPosition> segments, Direction direction)
    : segments_(std::move(segments)),
      direction_(direction)
{
    if (segments_.empty())
    {
        reset();
    }
}

const std::vector<GridPosition>& Snake::segments() const noexcept
{
    return segments_;
}

GridPosition Snake::head() const noexcept
{
    return segments_.front();
}

GridPosition Snake::nextHead() const noexcept
{
    GridPosition next = head();
    switch (direction_)
    {
    case Direction::Up: --next.y; break;
    case Direction::Down: ++next.y; break;
    case Direction::Left: --next.x; break;
    case Direction::Right: ++next.x; break;
    }
    return next;
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
    const GridPosition nextHeadPos = nextHead();
    // Keep the model on the configured grid until collision/game-over behavior
    // is triggered by the controller.
    if (!isWithinBoard(nextHeadPos))
    {
        return;
    }

    for (std::size_t index = segments_.size() - 1; index > 0; --index)
    {
        segments_[index] = segments_[index - 1];
    }
    segments_.front() = nextHeadPos;
}

void Snake::grow(std::size_t amount)
{
    if (segments_.empty())
    {
        return;
    }
    for (std::size_t i = 0; i < amount; ++i)
    {
        segments_.push_back(segments_.back());
    }
}

std::size_t Snake::length() const noexcept
{
    return segments_.size();
}

bool Snake::occupies(GridPosition position) const noexcept
{
    return std::find(segments_.begin(), segments_.end(), position) != segments_.end();
}

void Snake::reset()
{
    segments_ = {
        {config::BoardColumns / 2, config::BoardRows / 2},
        {config::BoardColumns / 2 - 1, config::BoardRows / 2},
        {config::BoardColumns / 2 - 2, config::BoardRows / 2}};
    direction_ = Direction::Right;
}
} // namespace snakearcade::model
