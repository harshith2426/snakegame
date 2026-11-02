#include "model/GameConfig.hpp"
#include "model/Snake.hpp"

#include <gtest/gtest.h>

#include <set>
#include <utility>

namespace snakearcade::model
{
TEST(SnakeTests, StartsWithThreeOrderedGridSegments)
{
    const Snake snake;
    const auto& segments = snake.segments();

    ASSERT_EQ(snake.length(), config::InitialSnakeLength);
    ASSERT_EQ(segments.size(), config::InitialSnakeLength);
    EXPECT_EQ(snake.head(), (GridPosition{config::BoardColumns / 2, config::BoardRows / 2}));
    EXPECT_EQ(segments[1], (GridPosition{config::BoardColumns / 2 - 1, config::BoardRows / 2}));
    EXPECT_EQ(segments[2], (GridPosition{config::BoardColumns / 2 - 2, config::BoardRows / 2}));
}

TEST(SnakeTests, StartsMovingRight)
{
    const Snake snake;

    EXPECT_EQ(snake.direction(), Direction::Right);
}

TEST(SnakeTests, EverySegmentIsInsideTheBoard)
{
    const Snake snake;

    for (const GridPosition segment : snake.segments())
    {
        EXPECT_TRUE(isWithinBoard(segment));
    }
}

TEST(SnakeTests, BoardCoordinateValidationIncludesEdgesOnly)
{
    EXPECT_TRUE(isWithinBoard({0, 0}));
    EXPECT_TRUE(isWithinBoard({config::BoardColumns - 1, config::BoardRows - 1}));
    EXPECT_FALSE(isWithinBoard({-1, 0}));
    EXPECT_FALSE(isWithinBoard({0, -1}));
    EXPECT_FALSE(isWithinBoard({config::BoardColumns, 0}));
    EXPECT_FALSE(isWithinBoard({0, config::BoardRows}));
}

TEST(SnakeTests, InitialSegmentsAreDistinctAndAdjacent)
{
    const Snake snake;
    std::set<std::pair<int, int>> occupied;

    for (std::size_t index = 0; index < snake.segments().size(); ++index)
    {
        const GridPosition segment = snake.segments()[index];
        EXPECT_TRUE(occupied.emplace(segment.x, segment.y).second);

        if (index > 0)
        {
            const GridPosition previous = snake.segments()[index - 1];
            EXPECT_EQ(previous.y, segment.y);
            EXPECT_EQ(previous.x - segment.x, 1);
        }
    }
}

TEST(SnakeTests, OccupancyQueryFindsBodyCells)
{
    const Snake snake;

    EXPECT_TRUE(snake.occupies(snake.head()));
    EXPECT_TRUE(snake.occupies(snake.segments().back()));
    EXPECT_FALSE(snake.occupies({0, 0}));
}

TEST(SnakeTests, MovesOneGridCellAndKeepsItsLength)
{
    Snake snake;
    snake.move();

    EXPECT_EQ(snake.head(), (GridPosition{config::BoardColumns / 2 + 1,
                                         config::BoardRows / 2}));
    EXPECT_EQ(snake.segments()[1], (GridPosition{config::BoardColumns / 2,
                                                config::BoardRows / 2}));
    EXPECT_EQ(snake.length(), config::InitialSnakeLength);
}

TEST(SnakeTests, RejectsImmediateReverseAndAllowsPerpendicularTurns)
{
    Snake snake;

    EXPECT_FALSE(snake.setDirection(Direction::Left));
    EXPECT_EQ(snake.direction(), Direction::Right);
    EXPECT_TRUE(snake.setDirection(Direction::Up));
    EXPECT_EQ(snake.direction(), Direction::Up);
    EXPECT_FALSE(snake.setDirection(Direction::Down));
}

TEST(SnakeTests, DoesNotMoveOutsideTheBoard)
{
    Snake snake;
    EXPECT_TRUE(snake.setDirection(Direction::Up));
    for (int step = 0; step < config::BoardRows; ++step)
    {
        snake.move();
    }

    EXPECT_TRUE(isWithinBoard(snake.head()));
    EXPECT_EQ(snake.head().y, 0);
    EXPECT_EQ(snake.segments()[1], (GridPosition{config::BoardColumns / 2, 1}));
}
} // namespace snakearcade::model
