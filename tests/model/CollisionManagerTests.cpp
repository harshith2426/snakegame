#include "model/CollisionManager.hpp"
#include "model/GameConfig.hpp"
#include "model/Snake.hpp"

#include <gtest/gtest.h>

#include <vector>

namespace snakearcade::model
{
TEST(CollisionManagerTests, NoWallCollisionWithinGridBoundaries)
{
    const CollisionManager manager;

    EXPECT_EQ(manager.checkWallCollision({0, 0}), Collision::None);
    EXPECT_EQ(manager.checkWallCollision({config::BoardColumns - 1, 0}), Collision::None);
    EXPECT_EQ(manager.checkWallCollision({0, config::BoardRows - 1}), Collision::None);
    EXPECT_EQ(manager.checkWallCollision({config::BoardColumns - 1, config::BoardRows - 1}), Collision::None);
    EXPECT_EQ(manager.checkWallCollision({config::BoardColumns / 2, config::BoardRows / 2}), Collision::None);
}

TEST(CollisionManagerTests, DetectsWallCollisionBeyondAllFourBoundaries)
{
    const CollisionManager manager;

    // Left wall (x < 0)
    EXPECT_EQ(manager.checkWallCollision({-1, 15}), Collision::Wall);
    // Right wall (x >= BoardColumns)
    EXPECT_EQ(manager.checkWallCollision({config::BoardColumns, 15}), Collision::Wall);
    // Top wall (y < 0)
    EXPECT_EQ(manager.checkWallCollision({20, -1}), Collision::Wall);
    // Bottom wall (y >= BoardRows)
    EXPECT_EQ(manager.checkWallCollision({20, config::BoardRows}), Collision::Wall);
}

TEST(CollisionManagerTests, SnakeOfLengthThreeHasNoSelfCollision)
{
    const CollisionManager manager;
    const Snake snake;

    EXPECT_EQ(manager.checkSelfCollision(snake.segments()), Collision::None);
    EXPECT_EQ(manager.checkCollision(snake), Collision::None);
}

TEST(CollisionManagerTests, DetectsSelfCollisionWhenHeadCollidesWithBody)
{
    const CollisionManager manager;

    // Create a snake of length 5 looping into itself:
    // Head at (5, 5) -> Body (5, 6) -> (4, 6) -> (4, 5) -> (3, 5)
    // Moving Left to (4, 5) collides with body segment at (4, 5)
    const std::vector<GridPosition> body = {
        {5, 5}, // Head
        {5, 6},
        {4, 6},
        {4, 5},
        {3, 5}  // Tail
    };

    const GridPosition nextHead{4, 5};
    EXPECT_EQ(manager.checkSelfCollision(nextHead, body, true), Collision::Self);
    EXPECT_EQ(manager.checkCollision(nextHead, body, true), Collision::Self);
}

TEST(CollisionManagerTests, MovingIntoVacatingTailIsNotSelfCollision)
{
    const CollisionManager manager;

    // A coiled 4-segment snake:
    // (5, 5) -> (6, 5) -> (6, 6) -> (5, 6)
    // Next head is (5, 6). When tail moves (normal move), (5, 6) is vacated.
    const std::vector<GridPosition> body = {
        {5, 5},
        {6, 5},
        {6, 6},
        {5, 6}  // Current tail
    };

    const GridPosition nextHead{5, 6};
    // If tail moves, this position is safe
    EXPECT_EQ(manager.checkSelfCollision(nextHead, body, true), Collision::None);
    EXPECT_EQ(manager.checkCollision(nextHead, body, true), Collision::None);

    // If tail is retained (growth), this position is a self collision
    EXPECT_EQ(manager.checkSelfCollision(nextHead, body, false), Collision::Self);
    EXPECT_EQ(manager.checkCollision(nextHead, body, false), Collision::Self);
}

TEST(CollisionManagerTests, PrioritizesWallCollisionOverSelfCollision)
{
    const CollisionManager manager;

    // If next head is outside the grid and also matches body (hypothetical), Wall takes precedence
    const std::vector<GridPosition> body = {{-1, 0}, {0, 0}, {1, 0}, {2, 0}};
    EXPECT_EQ(manager.checkCollision({-1, 0}, body), Collision::Wall);
}

TEST(CollisionManagerTests, WorksWithSnakeInstance)
{
    const CollisionManager manager;
    Snake snake;

    // Initial snake facing right at center: no collision
    EXPECT_EQ(manager.checkCollision(snake), Collision::None);

    // Turn up and move to top edge
    snake.setDirection(Direction::Up);
    for (int y = snake.head().y; y > 0; --y)
    {
        snake.move();
    }
    EXPECT_EQ(snake.head().y, 0);

    // Next move will hit the top wall
    EXPECT_EQ(manager.checkCollision(snake), Collision::Wall);
}
} // namespace snakearcade::model

