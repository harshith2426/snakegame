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

std::size_t Snake::length() const noexcept
{
    return segments_.size();
}

bool Snake::occupies(GridPosition position) const noexcept
{
    return std::find(segments_.begin(), segments_.end(), position) != segments_.end();
}
} // namespace snakearcade::model
