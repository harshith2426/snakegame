
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

TEST_F(HighScoreManagerTest, EmptyAndWhitespaceFilesLoadZero)
{
    {
        std::ofstream file(testFile);
        file << "";
    }
    snakearcade::persistence::HighScoreManager emptyManager(testFile.string());
    EXPECT_EQ(emptyManager.load(), 0);

    {
        std::ofstream file(testFile);
        file << "   \t\r\n  ";
    }
    snakearcade::persistence::HighScoreManager whitespaceManager(testFile.string());
    EXPECT_EQ(whitespaceManager.load(), 0);
}

TEST_F(HighScoreManagerTest, NegativeAndSignedValuesInFileLoadZero)
{
    {
        std::ofstream file(testFile);
        file << "-50";
    }
    snakearcade::persistence::HighScoreManager negativeManager(testFile.string());
    EXPECT_EQ(negativeManager.load(), 0);

    {
        std::ofstream file(testFile);
        file << "+100";
    }
    snakearcade::persistence::HighScoreManager plusManager(testFile.string());
    EXPECT_EQ(plusManager.load(), 0);
}

TEST_F(HighScoreManagerTest, OverflowAndDecimalValuesInFileLoadZero)
{
    {
        std::ofstream file(testFile);
        file << "999999999999999999999";
    }
    snakearcade::persistence::HighScoreManager overflowManager(testFile.string());
    EXPECT_EQ(overflowManager.load(), 0);

    {
        std::ofstream file(testFile);
        file << "12.5";
    }
    snakearcade::persistence::HighScoreManager decimalManager(testFile.string());
    EXPECT_EQ(decimalManager.load(), 0);
}

TEST_F(HighScoreManagerTest, TrimsValidWhitespaceAroundScore)
{
    {
        std::ofstream file(testFile);
        file << "  250 \r\n ";
    }
    snakearcade::persistence::HighScoreManager manager(testFile.string());
    EXPECT_EQ(manager.load(), 250);
}

TEST_F(HighScoreManagerTest, UnwritablePathDoesNotOverwriteInMemoryHighScore)
{
    // Pointing at an existing directory fails ofstream open
    const auto tempDir = std::filesystem::temp_directory_path();
    snakearcade::persistence::HighScoreManager manager(tempDir.string());

    EXPECT_EQ(manager.load(), 0);
    EXPECT_FALSE(manager.update(100));
    EXPECT_EQ(manager.highScore(), 0);
}
} // namespace
