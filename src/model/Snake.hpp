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

    // Segments are ordered from head to tail.
    const std::vector<GridPosition>& segments() const noexcept;
    GridPosition head() const noexcept;
    Direction direction() const noexcept;
    bool setDirection(Direction direction) noexcept;
    void move();
    std::size_t length() const noexcept;
    bool occupies(GridPosition position) const noexcept;

private:
    std::vector<GridPosition> segments_;
    Direction direction_;
};
} // namespace snakearcade::model
