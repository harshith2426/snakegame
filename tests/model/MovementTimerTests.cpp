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

TEST(MovementTimerTests, ResetDiscardsPartialMovementTime)
{
    MovementTimer timer(0.1F);
    EXPECT_EQ(timer.advance(0.08F), 0);
    timer.reset();
    EXPECT_EQ(timer.advance(0.03F), 0);
    EXPECT_EQ(timer.advance(0.07F), 1);
}

TEST(MovementTimerTests, SameElapsedTimeProducesSameTicksAtDifferentFrameRates)
{
    MovementTimer at60Hz(0.15F);
    MovementTimer at144Hz(0.15F);
    int ticksAt60Hz = 0;
    int ticksAt144Hz = 0;

    for (int frame = 0; frame < 60 * 8; ++frame)
    {
        ticksAt60Hz += at60Hz.advance(1.0F / 60.0F);
    }

    for (int frame = 0; frame < 144 * 8; ++frame)
    {
        ticksAt144Hz += at144Hz.advance(1.0F / 144.0F);
    }

    EXPECT_EQ(ticksAt60Hz, 53);
    EXPECT_EQ(ticksAt144Hz, 53);
    EXPECT_EQ(ticksAt60Hz, ticksAt144Hz);
}
} // namespace snakearcade::model
