#pragma once

#include "model/GameTypes.hpp"

#include <cstddef>
#include <vector>

namespace snakearcade::model
{
class Snake final
{
public:
    Snake();
    explicit Snake(std::vector<GridPosition> segments, Direction direction = Direction::Right);

    // Segments are ordered from head to tail.
    const std::vector<GridPosition>& segments() const noexcept;
    GridPosition head() const noexcept;
    GridPosition nextHead() const noexcept;
    Direction direction() const noexcept;
    bool setDirection(Direction direction) noexcept;
    void move();
    void grow(std::size_t amount = 1);
    std::size_t length() const noexcept;
    bool occupies(GridPosition position) const noexcept;
    void reset();

private:
    std::vector<GridPosition> segments_;
    Direction direction_;
};
} // namespace snakearcade::model
