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

TEST(GameplaySessionTests, EasyDifficultyUsesSlowMovement)
{
    Snake snake;
    GameplaySession session(snake, GridPosition{0, 0}, 17, Difficulty::Easy);

    session.advance(0.19F);
    EXPECT_EQ(session.snake().head(), snake.head());

    session.advance(0.01F);
    EXPECT_NE(session.snake().head(), snake.head());
}

TEST(GameplaySessionTests, MediumDifficultyUsesNormalMovement)
{
    Snake snake;
    GameplaySession session(snake, GridPosition{0, 0}, 17, Difficulty::Medium);

    session.advance(0.149F);
    EXPECT_EQ(session.snake().head(), snake.head());

    session.advance(0.001F);
    EXPECT_NE(session.snake().head(), snake.head());
}

TEST(GameplaySessionTests, HardDifficultyUsesFastMovement)
{
    Snake snake;
    GameplaySession session(snake, GridPosition{0, 0}, 17, Difficulty::Hard);

    session.advance(0.079F);
    EXPECT_EQ(session.snake().head(), snake.head());

    session.advance(0.001F);
    EXPECT_NE(session.snake().head(), snake.head());
}

TEST(GameplaySessionTests, RestartPreservesSelectedDifficultyAndClearsPause)
{
    GameplaySession session(Difficulty::Hard);

    session.pause();
    EXPECT_TRUE(session.isPaused());
    session.restart();

    EXPECT_EQ(session.difficulty(), Difficulty::Hard);
    EXPECT_FALSE(session.isPaused());
}

TEST(GameplaySessionTests, PauseStopsMovementAndInputWhilePreservingTimerRemainder)
{
    Snake snake;
    GameplaySession session(snake, GridPosition{0, 0}, 17, Difficulty::Medium);

    // Accumulate 0.10s (short of 0.15s tick)
    session.advance(0.10F);
    EXPECT_EQ(session.snake().head(), snake.head());

    session.pause();
    EXPECT_TRUE(session.isPaused());

    // Time and direction changes while paused must be ignored
    session.requestDirection(Direction::Up);
    session.advance(5.0F);
    EXPECT_EQ(session.snake().head(), snake.head());
    EXPECT_EQ(session.snake().direction(), Direction::Right);

    // Resuming preserves the 0.10s accumulator so 0.05s triggers the tick
    session.resume();
    EXPECT_FALSE(session.isPaused());
    session.advance(0.05F);
    EXPECT_EQ(session.snake().head(), (GridPosition{snake.head().x + 1, snake.head().y}));
}

TEST(GameplaySessionTests, RestartWithNewDifficultyUpdatesMovementInterval)
{
    Snake snake;
    GameplaySession session(snake, GridPosition{0, 0}, 17, Difficulty::Easy);

    session.restart(Difficulty::Hard);
    EXPECT_EQ(session.difficulty(), Difficulty::Hard);

    session.advance(0.08F);
    EXPECT_NE(session.snake().head(), snake.head());
}

TEST(GameplaySessionTests, RapidTwoKeyTurnWithinSingleTickCannotReverseIntoNeck)
{
    Snake snake;
    GameplaySession session(snake, GridPosition{0, 0}, 17, Difficulty::Medium);

    // Snake faces Right with neck at (head.x - 1, head.y)
    session.requestDirection(Direction::Up);
    session.requestDirection(Direction::Left);

    session.advance(0.15F);
    EXPECT_FALSE(session.isGameOver());
    EXPECT_EQ(session.snake().head(), (GridPosition{snake.head().x, snake.head().y - 1}));
}

TEST(GameplaySessionTests, SelfCollisionDetectedOnRetainedTailImmediatelyAfterGrowth)
{
    // 4-segment coiled snake about to eat food at (5, 5), then turn into its own retained tail at (5, 6)
    Snake snake({{4, 5}, {4, 6}, {5, 6}, {6, 6}}, Direction::Right);
    GameplaySession session(snake, GridPosition{5, 5}, 17, Difficulty::Medium);

    // Move onto food at (5, 5) -> grows by retaining tail (6, 6)
    session.advance(0.15F);
    EXPECT_FALSE(session.isGameOver());
    EXPECT_EQ(session.score(), config::PointsPerFood);
    EXPECT_EQ(session.snake().length(), 5U);

    // Turn down towards (5, 6), which is still occupied by body segment
    session.requestDirection(Direction::Down);
    session.advance(0.15F);
    EXPECT_TRUE(session.isGameOver());
}
} // namespace snakearcade::model
