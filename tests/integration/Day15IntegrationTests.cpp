#include "controller/GameCoordinator.hpp"
#include "model/GameConfig.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace
{
using snakearcade::model::Difficulty;
using snakearcade::model::Direction;
using snakearcade::model::GameState;
using snakearcade::model::GameplaySession;
using snakearcade::model::GridPosition;
using snakearcade::model::Snake;

class Day15IntegrationTest : public ::testing::Test
{
protected:
    std::filesystem::path scoreFile;

    void SetUp() override
    {
        scoreFile = std::filesystem::temp_directory_path()
                    / "snakearcade_day15_integration_highscore.txt";
        std::filesystem::remove(scoreFile);
    }

    void TearDown() override
    {
        std::filesystem::remove(scoreFile);
    }
};

TEST_F(Day15IntegrationTest, StartsInStartStateAndMissingFileLoadsZeroWithoutWriting)
{
    const GameCoordinator coordinator(scoreFile.string());

    EXPECT_EQ(coordinator.state(), GameState::Start);
    EXPECT_EQ(coordinator.difficulty(), Difficulty::Medium);
    EXPECT_EQ(coordinator.highScore(), 0);
    EXPECT_FALSE(coordinator.isPlaying());
    EXPECT_FALSE(coordinator.isPaused());
    EXPECT_FALSE(coordinator.isGameOver());
    EXPECT_FALSE(std::filesystem::exists(scoreFile));
}

TEST_F(Day15IntegrationTest, PausePreservesSessionAndRestartResetsSession)
{
    GameCoordinator coordinator(scoreFile.string());

    Snake snake;
    const GridPosition firstFood{snake.head().x + 1, snake.head().y};
    ASSERT_TRUE(coordinator.startCustomSession(
        GameplaySession(snake, firstFood, 42, Difficulty::Medium)));
    EXPECT_EQ(coordinator.state(), GameState::Playing);

    // Eat first food (0.15s)
    coordinator.update(0.15F);
    ASSERT_EQ(coordinator.score(), snakearcade::config::PointsPerFood);
    const GridPosition headAfterFood = coordinator.session().snake().head();
    const std::size_t lengthAfterFood = coordinator.session().snake().length();
    const auto nextFood = coordinator.session().food();

    // Accumulate partial tick, then pause
    coordinator.update(0.10F);
    ASSERT_TRUE(coordinator.pause());
    EXPECT_EQ(coordinator.state(), GameState::Paused);
    EXPECT_TRUE(coordinator.isPaused());

    // Elapsed time and direction input must do nothing while paused
    coordinator.requestDirection(Direction::Up);
    coordinator.update(3.0F);
    EXPECT_EQ(coordinator.session().snake().head(), headAfterFood);
    EXPECT_EQ(coordinator.session().snake().length(), lengthAfterFood);
    EXPECT_EQ(coordinator.session().snake().direction(), Direction::Right);
    EXPECT_EQ(coordinator.session().food(), nextFood);

    // Resume preserves accumulated 0.10s, so 0.05s triggers next step
    ASSERT_TRUE(coordinator.resume());
    EXPECT_EQ(coordinator.state(), GameState::Playing);
    coordinator.update(0.05F);
    EXPECT_EQ(coordinator.session().snake().head(),
              (GridPosition{headAfterFood.x + 1, headAfterFood.y}));

    // Drive into right wall to reach GameOver
    for (int step = 0; step < snakearcade::config::BoardColumns; ++step)
    {
        coordinator.update(0.15F);
        if (coordinator.isGameOver())
        {
            break;
        }
    }
    ASSERT_EQ(coordinator.state(), GameState::GameOver);
    EXPECT_TRUE(coordinator.isGameOver());

    // Restart resets snake, score, and state while preserving difficulty
    ASSERT_TRUE(coordinator.restart());
    EXPECT_EQ(coordinator.state(), GameState::Playing);
    EXPECT_EQ(coordinator.difficulty(), Difficulty::Medium);
    EXPECT_EQ(coordinator.score(), 0);
    EXPECT_EQ(coordinator.session().snake().length(), snakearcade::config::InitialSnakeLength);
    EXPECT_EQ(coordinator.session().snake().head(), snake.head());
    EXPECT_FALSE(coordinator.isGameOver());
}

TEST_F(Day15IntegrationTest, RejectsInvalidTransitionsAndInputsWithoutMutatingSession)
{
    GameCoordinator coordinator(scoreFile.string());

    // Start state rejections
    EXPECT_FALSE(coordinator.pause());
    EXPECT_FALSE(coordinator.resume());
    EXPECT_FALSE(coordinator.togglePause());
    EXPECT_FALSE(coordinator.restart());
    EXPECT_FALSE(coordinator.returnToStart());
    coordinator.requestDirection(Direction::Up);
    coordinator.update(1.0F);
    EXPECT_EQ(coordinator.state(), GameState::Start);
    EXPECT_EQ(coordinator.session().snake().direction(), Direction::Right);

    // Playing state rejections
    ASSERT_TRUE(coordinator.startSession(Difficulty::Easy));
    EXPECT_FALSE(coordinator.selectDifficulty(Difficulty::Hard));
    EXPECT_FALSE(coordinator.startSession(Difficulty::Hard));
    EXPECT_FALSE(coordinator.resume());
    EXPECT_FALSE(coordinator.restart());
    EXPECT_FALSE(coordinator.returnToStart());
    EXPECT_EQ(coordinator.difficulty(), Difficulty::Easy);

    // Paused state rejections
    ASSERT_TRUE(coordinator.pause());
    EXPECT_FALSE(coordinator.selectDifficulty(Difficulty::Hard));
    EXPECT_FALSE(coordinator.pause());
    EXPECT_FALSE(coordinator.restart());
    EXPECT_FALSE(coordinator.returnToStart());
}

TEST_F(Day15IntegrationTest, DifficultySelectionControlsSpeedAndSupportsReturnToStart)
{
    GameCoordinator coordinator(scoreFile.string());

    // Select Easy (0.20s)
    ASSERT_TRUE(coordinator.selectDifficulty(Difficulty::Easy));
    ASSERT_TRUE(coordinator.startSession());
    const GridPosition startHead = coordinator.session().snake().head();

    coordinator.update(0.19F);
    EXPECT_EQ(coordinator.session().snake().head(), startHead);
    coordinator.update(0.01F);
    EXPECT_NE(coordinator.session().snake().head(), startHead);

    // Drive to GameOver on top wall
    coordinator.requestDirection(Direction::Up);
    for (int step = 0; step < snakearcade::config::BoardRows; ++step)
    {
        coordinator.update(0.20F);
    }
    ASSERT_EQ(coordinator.state(), GameState::GameOver);

    // Return to Start and switch to Hard (0.08s)
    ASSERT_TRUE(coordinator.returnToStart());
    EXPECT_EQ(coordinator.state(), GameState::Start);
    ASSERT_TRUE(coordinator.selectDifficulty(Difficulty::Hard));
    ASSERT_TRUE(coordinator.startSession());
    EXPECT_EQ(coordinator.difficulty(), Difficulty::Hard);

    coordinator.update(0.079F);
    EXPECT_EQ(coordinator.session().snake().head(), startHead);
    coordinator.update(0.001F);
    EXPECT_NE(coordinator.session().snake().head(), startHead);
}

TEST_F(Day15IntegrationTest, HighScoreUpdatesAndPersistsAcrossSessionsOnlyWhenBeaten)
{
    {
        GameCoordinator coordinator(scoreFile.string());

        // Snake one step left of right wall, eating food on its last valid cell then hitting wall
        Snake snake({{snakearcade::config::BoardColumns - 2, 5},
                     {snakearcade::config::BoardColumns - 3, 5},
                     {snakearcade::config::BoardColumns - 4, 5}},
                    Direction::Right);
        const GridPosition food{snakearcade::config::BoardColumns - 1, 5};
        ASSERT_TRUE(coordinator.startCustomSession(
            GameplaySession(snake, food, 99, Difficulty::Hard)));

        coordinator.update(0.08F);
        EXPECT_FALSE(coordinator.isGameOver());
        EXPECT_EQ(coordinator.score(), snakearcade::config::PointsPerFood);

        coordinator.update(0.08F);
        EXPECT_TRUE(coordinator.isGameOver());
        EXPECT_EQ(coordinator.highScore(), snakearcade::config::PointsPerFood);

        // Subsequent updates while GameOver do not alter snake or score
        const auto frozenSegments = coordinator.session().snake().segments();
        coordinator.update(1.0F);
        EXPECT_EQ(coordinator.session().snake().segments(), frozenSegments);
        EXPECT_EQ(coordinator.highScore(), snakearcade::config::PointsPerFood);
    }

    // Reload in a fresh coordinator instance to verify disk persistence
    GameCoordinator reloaded(scoreFile.string());
    EXPECT_EQ(reloaded.highScore(), snakearcade::config::PointsPerFood);

    // Finish another session with score 0 -> high score stays 10
    Snake edgeSnake({{0, 5}, {1, 5}, {2, 5}}, Direction::Left);
    ASSERT_TRUE(reloaded.startCustomSession(
        GameplaySession(edgeSnake, GridPosition{10, 10}, 99, Difficulty::Medium)));
    reloaded.update(0.15F);
    ASSERT_TRUE(reloaded.isGameOver());
    EXPECT_EQ(reloaded.score(), 0);
    EXPECT_EQ(reloaded.highScore(), snakearcade::config::PointsPerFood);
}

TEST_F(Day15IntegrationTest, MalformedScoreFileLoadsZeroAndRecoversWhenBeaten)
{
    {
        std::ofstream badFile(scoreFile);
        badFile << "corrupted-score-data";
    }

    GameCoordinator coordinator(scoreFile.string());
    EXPECT_EQ(coordinator.highScore(), 0);

    // Startup must not clobber the malformed file before a higher score is earned
    {
        std::ifstream verifyUnchanged(scoreFile);
        std::string contents;
        std::getline(verifyUnchanged, contents);
        EXPECT_EQ(contents, "corrupted-score-data");
    }

    // Beat high score (0 -> 10) and hit wall
    Snake snake({{snakearcade::config::BoardColumns - 2, 10},
                 {snakearcade::config::BoardColumns - 3, 10},
                 {snakearcade::config::BoardColumns - 4, 10}},
                Direction::Right);
    const GridPosition food{snakearcade::config::BoardColumns - 1, 10};
    ASSERT_TRUE(coordinator.startCustomSession(
        GameplaySession(snake, food, 7, Difficulty::Medium)));

    coordinator.update(0.15F);
    coordinator.update(0.15F);
    ASSERT_TRUE(coordinator.isGameOver());
    EXPECT_EQ(coordinator.highScore(), snakearcade::config::PointsPerFood);

    // Verify file now persists valid score cleanly
    GameCoordinator verifiedReload(scoreFile.string());
    EXPECT_EQ(verifiedReload.highScore(), snakearcade::config::PointsPerFood);
}
} // namespace

