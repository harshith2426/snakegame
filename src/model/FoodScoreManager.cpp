#include "model/FoodScoreManager.hpp"
#include "model/GameConfig.hpp"

#include <algorithm>

namespace snakearcade::model
{
FoodScoreManager::FoodScoreManager()
    : rng_(std::random_device{}())
{
}

FoodScoreManager::FoodScoreManager(unsigned int seed)
    : rng_(seed)
{
}

std::optional<GridPosition> FoodScoreManager::spawnFood(const std::vector<GridPosition>& occupied)
{
    std::vector<GridPosition> available;
    available.reserve(static_cast<std::size_t>(config::BoardColumns * config::BoardRows));

    std::vector<std::vector<bool>> isOccupied(
        static_cast<std::size_t>(config::BoardColumns),
        std::vector<bool>(static_cast<std::size_t>(config::BoardRows), false));

    for (const auto& pos : occupied)
    {
        if (isWithinBoard(pos))
        {
            isOccupied[static_cast<std::size_t>(pos.x)][static_cast<std::size_t>(pos.y)] = true;
        }
    }

    for (int x = 0; x < config::BoardColumns; ++x)
    {
        for (int y = 0; y < config::BoardRows; ++y)
        {
            if (!isOccupied[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)])
            {
                available.push_back({x, y});
            }
        }
    }

    if (available.empty())
    {
        return std::nullopt;
    }

    std::uniform_int_distribution<std::size_t> dist(0, available.size() - 1);
    return available[dist(rng_)];
}

std::optional<GridPosition> FoodScoreManager::spawnFood(const Snake& snake)
{
    return spawnFood(snake.segments());
}

bool FoodScoreManager::isConsumed(GridPosition head, GridPosition food) const noexcept
{
    return head == food;
}

int FoodScoreManager::applyScore(int currentScore, int points) const noexcept
{
    return currentScore + points;
}
} // namespace snakearcade::model

