#pragma once

#include "model/GameConfig.hpp"
#include "model/GameTypes.hpp"
#include "model/Snake.hpp"

#include <optional>
#include <random>
#include <vector>

namespace snakearcade::model
{
class FoodScoreManager final
{
public:
    FoodScoreManager();
    explicit FoodScoreManager(unsigned int seed);

    std::optional<GridPosition> spawnFood(const std::vector<GridPosition>& occupied);
    std::optional<GridPosition> spawnFood(const Snake& snake);

    bool isConsumed(GridPosition head, GridPosition food) const noexcept;
    int applyScore(int currentScore, int points = config::PointsPerFood) const noexcept;

private:
    std::mt19937 rng_;
};
} // namespace snakearcade::model

