#include "model/MovementTimer.hpp"

#include <gtest/gtest.h>

#include <limits>

namespace snakearcade::model
{
TEST(MovementTimerTests, DoesNotMoveBeforeInterval)
{
    MovementTimer timer(0.1F);
    EXPECT_EQ(timer.advance(0.04F), 0);
    EXPECT_EQ(timer.advance(0.05F), 0);
    EXPECT_EQ(timer.advance(0.01F), 1);
}

TEST(MovementTimerTests, PreservesRemainderAndCatchesUpElapsedTime)
{
    MovementTimer timer(0.1F);
    EXPECT_EQ(timer.advance(0.25F), 2);
    EXPECT_EQ(timer.advance(0.05F), 1);
}

TEST(MovementTimerTests, IgnoresInvalidOrNegativeElapsedTime)
{
    MovementTimer timer(0.1F);
    EXPECT_EQ(timer.advance(-1.0F), 0);
    EXPECT_EQ(timer.advance(std::numeric_limits<float>::infinity()), 0);
    EXPECT_EQ(timer.advance(0.1F), 1);
}
} // namespace snakearcade::model
