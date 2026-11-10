
#include "persistence/HighScoreManager.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace
{
class HighScoreManagerTest : public ::testing::Test
{
protected:
    std::filesystem::path testFile;

    void SetUp() override
    {
        testFile = std::filesystem::temp_directory_path()
                   / "snakearcade_highscore_test.txt";

        std::filesystem::remove(testFile);
    }

    void TearDown() override
    {
        std::filesystem::remove(testFile);
    }
};

TEST_F(HighScoreManagerTest, MissingFileLoadsZero)
{
    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_EQ(manager.load(), 0);
}

TEST_F(HighScoreManagerTest, SavesAndLoadsHighScore)
{
    {
        snakearcade::persistence::HighScoreManager manager(
            testFile.string()
        );

        EXPECT_TRUE(manager.update(100));
        EXPECT_EQ(manager.highScore(), 100);
    }

    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_EQ(manager.load(), 100);
}

TEST_F(HighScoreManagerTest, LowerScoreDoesNotReplaceHighScore)
{
    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_TRUE(manager.update(100));
    EXPECT_FALSE(manager.update(50));

    EXPECT_EQ(manager.highScore(), 100);
}

TEST_F(HighScoreManagerTest, EqualScoreDoesNotReplaceHighScore)
{
    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_TRUE(manager.update(100));
    EXPECT_FALSE(manager.update(100));

    EXPECT_EQ(manager.highScore(), 100);
}

TEST_F(HighScoreManagerTest, HigherScoreUpdatesHighScore)
{
    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_TRUE(manager.update(100));
    EXPECT_TRUE(manager.update(150));

    EXPECT_EQ(manager.highScore(), 150);
}

TEST_F(HighScoreManagerTest, MalformedFileLoadsZero)
{
    {
        std::ofstream file(testFile);
        file << "invalid-score";
    }

    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_EQ(manager.load(), 0);
}

TEST_F(HighScoreManagerTest, NegativeScoreIsRejected)
{
    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_FALSE(manager.update(-10));
    EXPECT_EQ(manager.highScore(), 0);
}

TEST_F(HighScoreManagerTest, ExtraDataIsRejected)
{
    {
        std::ofstream file(testFile);
        file << "100 extra";
    }

    snakearcade::persistence::HighScoreManager manager(
        testFile.string()
    );

    EXPECT_EQ(manager.load(), 0);
}
} // namespace
