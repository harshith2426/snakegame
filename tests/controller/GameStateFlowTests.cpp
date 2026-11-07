#include "controller/GameStateFlow.hpp"

#include <gtest/gtest.h>

TEST(GameStateFlowTests, StartsInStartStateWithMediumDifficulty)
{
    const GameStateFlow flow;
    EXPECT_EQ(flow.state(), GameStateFlow::State::Start);
    EXPECT_EQ(flow.difficulty(), GameStateFlow::Difficulty::Medium);
    EXPECT_FALSE(flow.isPlaying());
}

TEST(GameStateFlowTests, StartsSessionAndStoresSelectedDifficulty)
{
    GameStateFlow flow;
    ASSERT_TRUE(flow.startSession(GameStateFlow::Difficulty::Easy));
    EXPECT_EQ(flow.state(), GameStateFlow::State::Playing);
    EXPECT_EQ(flow.difficulty(), GameStateFlow::Difficulty::Easy);
    EXPECT_TRUE(flow.isPlaying());
}

TEST(GameStateFlowTests, AllowsPauseAndResumeTransitions)
{
    GameStateFlow flow;
    ASSERT_TRUE(flow.startSession(GameStateFlow::Difficulty::Medium));
    ASSERT_TRUE(flow.pause());
    EXPECT_EQ(flow.state(), GameStateFlow::State::Paused);
    EXPECT_FALSE(flow.isPlaying());
    ASSERT_TRUE(flow.resume());
    EXPECT_EQ(flow.state(), GameStateFlow::State::Playing);
    EXPECT_TRUE(flow.isPlaying());
}

TEST(GameStateFlowTests, FinishesOnlyAnActiveSession)
{
    GameStateFlow flow;
    EXPECT_FALSE(flow.finishSession());
    ASSERT_TRUE(flow.startSession(GameStateFlow::Difficulty::Hard));
    EXPECT_TRUE(flow.finishSession());
    EXPECT_EQ(flow.state(), GameStateFlow::State::GameOver);
    EXPECT_FALSE(flow.isPlaying());
    EXPECT_FALSE(flow.finishSession());
}

TEST(GameStateFlowTests, CanRestartAfterGameOver)
{
    GameStateFlow flow;
    ASSERT_TRUE(flow.startSession(GameStateFlow::Difficulty::Easy));
    ASSERT_TRUE(flow.finishSession());
    ASSERT_TRUE(flow.startSession(GameStateFlow::Difficulty::Hard));
    EXPECT_EQ(flow.state(), GameStateFlow::State::Playing);
    EXPECT_EQ(flow.difficulty(), GameStateFlow::Difficulty::Hard);
}

TEST(GameStateFlowTests, RejectsInvalidTransitionsWithoutChangingState)
{
    GameStateFlow flow;
    EXPECT_FALSE(flow.pause());
    EXPECT_FALSE(flow.resume());
    EXPECT_FALSE(flow.finishSession());
    EXPECT_FALSE(flow.returnToStart());
    EXPECT_EQ(flow.state(), GameStateFlow::State::Start);

    ASSERT_TRUE(flow.startSession(GameStateFlow::Difficulty::Medium));
    EXPECT_FALSE(flow.startSession(GameStateFlow::Difficulty::Hard));
    EXPECT_FALSE(flow.resume());
    EXPECT_FALSE(flow.returnToStart());
    EXPECT_EQ(flow.state(), GameStateFlow::State::Playing);
}

TEST(GameStateFlowTests, ReturnsToStartOnlyFromGameOver)
{
    GameStateFlow flow;
    ASSERT_TRUE(flow.startSession(GameStateFlow::Difficulty::Medium));
    EXPECT_FALSE(flow.returnToStart());
    ASSERT_TRUE(flow.finishSession());
    EXPECT_TRUE(flow.returnToStart());
    EXPECT_EQ(flow.state(), GameStateFlow::State::Start);
}
