#include "model/FoodScoreManager.hpp"
#include "model/GameConfig.hpp"
#include "model/Snake.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <vector>

namespace snakearcade::model
{
TEST(FoodScoreManagerTests, SpawnsFoodAtValidGridPosition)
{
    FoodScoreManager manager(12345);
    const Snake snake;

    const std::optional<GridPosition> food = manager.spawnFood(snake.segments());

    ASSERT_TRUE(food.has_value());
    EXPECT_TRUE(isWithinBoard(*food));
}

TEST(FoodScoreManagerTests, FoodNeverSpawnsOnSnakeBody)
{
    FoodScoreManager manager(42);
    const Snake snake;

    // Test across multiple runs to verify randomized placement never overlaps
    for (int iteration = 0; iteration < 50; ++iteration)
    {
        const std::optional<GridPosition> food = manager.spawnFood(snake.segments());
        ASSERT_TRUE(food.has_value());
        EXPECT_TRUE(isWithinBoard(*food));
        EXPECT_FALSE(snake.occupies(*food));
    }
}

TEST(FoodScoreManagerTests, ReturnsNulloptWhenBoardIsFull)
{
    FoodScoreManager manager(100);
    std::vector<GridPosition> allCells;
    allCells.reserve(static_cast<std::size_t>(config::BoardColumns * config::BoardRows));

    for (int x = 0; x < config::BoardColumns; ++x)
    {
        for (int y = 0; y < config::BoardRows; ++y)
        {
            allCells.push_back({x, y});
        }
    }

    const std::optional<GridPosition> food = manager.spawnFood(allCells);
    EXPECT_FALSE(food.has_value());
}

TEST(FoodScoreManagerTests, SpawnsInTheOnlyAvailableCell)
{
    FoodScoreManager manager(999);
    std::vector<GridPosition> almostAllCells;
    const GridPosition onlyFreeCell{7, 11};

    for (int x = 0; x < config::BoardColumns; ++x)
    {
        for (int y = 0; y < config::BoardRows; ++y)
        {
            if (x != onlyFreeCell.x || y != onlyFreeCell.y)
            {
                almostAllCells.push_back({x, y});
            }
        }
    }

    const std::optional<GridPosition> food = manager.spawnFood(almostAllCells);
    ASSERT_TRUE(food.has_value());
    EXPECT_EQ(*food, onlyFreeCell);
}

TEST(FoodScoreManagerTests, DetectsConsumptionWhenHeadMatchesFoodPosition)
{
    const FoodScoreManager manager;
    const GridPosition food{15, 10};

    EXPECT_TRUE(manager.isConsumed({15, 10}, food));
    EXPECT_FALSE(manager.isConsumed({14, 10}, food));
    EXPECT_FALSE(manager.isConsumed({15, 11}, food));
    EXPECT_FALSE(manager.isConsumed({0, 0}, food));
}

TEST(FoodScoreManagerTests, IncreasesScoreWhenFoodIsConsumed)
{
    const FoodScoreManager manager;

    EXPECT_EQ(manager.applyScore(0), config::PointsPerFood);
    EXPECT_EQ(manager.applyScore(config::PointsPerFood), config::PointsPerFood * 2);
    EXPECT_EQ(manager.applyScore(100), 100 + config::PointsPerFood);
}

TEST(FoodScoreManagerTests, SeededGeneratorIsDeterministic)
{
    FoodScoreManager manager1(777);
    FoodScoreManager manager2(777);
    const Snake snake;

    for (int i = 0; i < 10; ++i)
    {
        const auto food1 = manager1.spawnFood(snake.segments());
        const auto food2 = manager2.spawnFood(snake.segments());
        ASSERT_TRUE(food1.has_value());
        ASSERT_TRUE(food2.has_value());
        EXPECT_EQ(*food1, *food2);
    }
}

TEST(FoodScoreManagerTests, CompleteConsumptionCycleGrowsSnakeAndIncreasesScore)
{
    FoodScoreManager manager(54321);
    Snake snake;
    int score = 0;

    const std::size_t initialLength = snake.length();

    // Place food directly in front of snake
    const GridPosition targetFood{snake.head().x + 1, snake.head().y};
    ASSERT_TRUE(isWithinBoard(targetFood));

    // Snake moves onto the food
    snake.move();
    EXPECT_EQ(snake.head(), targetFood);

    // Consumption detected
    ASSERT_TRUE(manager.isConsumed(snake.head(), targetFood));

    // Grow snake and update score
    snake.grow();
    score = manager.applyScore(score);

    EXPECT_EQ(snake.length(), initialLength + 1);
    EXPECT_EQ(score, config::PointsPerFood);

    // Spawn next food position and verify it doesn't collide with expanded snake
    const auto nextFood = manager.spawnFood(snake.segments());
    ASSERT_TRUE(nextFood.has_value());
    EXPECT_TRUE(isWithinBoard(*nextFood));
    EXPECT_FALSE(snake.occupies(*nextFood));
}
} // namespace snakearcade::model

