#include "model/GameConfig.hpp"
#include "model/GameplaySession.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <vector>

namespace snakearcade::model
{
TEST(GameplaySessionTests, StartsWithSnakeFoodAndZeroScore)
{
    GameplaySession session;

    EXPECT_EQ(session.snake().length(), config::InitialSnakeLength);
    ASSERT_TRUE(session.food().has_value());
    EXPECT_TRUE(isWithinBoard(*session.food()));
    EXPECT_FALSE(session.snake().occupies(*session.food()));
    EXPECT_EQ(session.score(), 0);
    EXPECT_FALSE(session.isFinished());
}

TEST(GameplaySessionTests, CoordinatesMovementFoodGrowthAndScoring)
{
    Snake snake;
    const GridPosition food{snake.head().x + 1, snake.head().y};
    GameplaySession session(snake, food, 17);

    session.advance(0.149F);
    EXPECT_EQ(session.snake().head(), snake.head());
    EXPECT_EQ(session.score(), 0);

    session.advance(0.001F);
    EXPECT_EQ(session.snake().head(), food);
    EXPECT_EQ(session.snake().length(), config::InitialSnakeLength + 1);
    EXPECT_EQ(session.score(), config::PointsPerFood);
    ASSERT_TRUE(session.food().has_value());
    EXPECT_TRUE(isWithinBoard(*session.food()));
    EXPECT_FALSE(session.snake().occupies(*session.food()));
}

TEST(GameplaySessionTests, WallCollisionEndsSessionWithoutMovingOutsideBoard)
{
    Snake snake({{config::BoardColumns - 1, 4}, {config::BoardColumns - 2, 4},
                 {config::BoardColumns - 3, 4}}, Direction::Right);
    GameplaySession session(snake, GridPosition{0, 0}, 17);

    session.advance(0.15F);

    EXPECT_TRUE(session.isGameOver());
    EXPECT_TRUE(session.isFinished());
    EXPECT_FALSE(session.isWon());
    EXPECT_EQ(session.snake().head(), (GridPosition{config::BoardColumns - 1, 4}));
    EXPECT_EQ(session.score(), 0);
}

TEST(GameplaySessionTests, SelfCollisionEndsSessionWithoutAwardingFood)
{
    Snake snake({{5, 5}, {5, 6}, {4, 6}, {4, 5}, {3, 5}}, Direction::Left);
    GameplaySession session(snake, GridPosition{0, 0}, 17);

    session.advance(0.15F);

    EXPECT_TRUE(session.isGameOver());
    EXPECT_EQ(session.snake().head(), (GridPosition{5, 5}));
    EXPECT_EQ(session.score(), 0);
}

TEST(GameplaySessionTests, RejectsImmediateReverseAndDoesNotAdvanceAfterGameOver)
{
    Snake snake({{config::BoardColumns - 1, 4}, {config::BoardColumns - 2, 4},
                 {config::BoardColumns - 3, 4}}, Direction::Right);
    GameplaySession session(snake, GridPosition{0, 0}, 17);

    session.requestDirection(Direction::Left);
    EXPECT_EQ(session.snake().direction(), Direction::Right);
    session.advance(0.15F);
    const auto finalSnake = session.snake().segments();
    session.advance(1.0F);

    EXPECT_EQ(session.snake().segments(), finalSnake);
    EXPECT_TRUE(session.isGameOver());
}

TEST(GameplaySessionTests, RestartResetsSnakeScoreFoodAndMovementTimer)
{
    Snake snake;
    GameplaySession session(snake, GridPosition{snake.head().x + 1, snake.head().y}, 17);
    session.advance(0.15F);
    ASSERT_EQ(session.score(), config::PointsPerFood);

    session.restart();

    EXPECT_EQ(session.snake().head(), snake.head());
    EXPECT_EQ(session.snake().length(), config::InitialSnakeLength);
    EXPECT_EQ(session.score(), 0);
    EXPECT_FALSE(session.isFinished());
    ASSERT_TRUE(session.food().has_value());
    EXPECT_FALSE(session.snake().occupies(*session.food()));
    session.advance(0.149F);
    EXPECT_EQ(session.snake().head(), snake.head());
}

TEST(GameplaySessionTests, FullBoardProducesWinInsteadOfMissingFoodState)
{
    std::vector<GridPosition> allCells;
    allCells.reserve(static_cast<std::size_t>(config::BoardColumns * config::BoardRows));
    for (int x = 0; x < config::BoardColumns; ++x)
    {
        for (int y = 0; y < config::BoardRows; ++y)
        {
            allCells.push_back({x, y});
        }
    }
    GameplaySession session(Snake(std::move(allCells)), std::nullopt, 17);

    EXPECT_FALSE(session.food().has_value());
    EXPECT_TRUE(session.isWon());
    EXPECT_TRUE(session.isFinished());
    EXPECT_FALSE(session.isGameOver());
}
} // namespace snakearcade::model
