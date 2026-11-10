#include <utility>
#include "persistence/HighScoreManager.hpp"

#include <fstream>
#include <limits>

namespace snakearcade::persistence
{
HighScoreManager::HighScoreManager(std::string filePath)
    : filePath_(std::move(filePath))
{
}

int HighScoreManager::load() noexcept
{
    highScore_ = 0;

    std::ifstream file(filePath_);

    if (!file.is_open())
    {
        return highScore_;
    }

    long long storedScore = 0;

    if (!(file >> storedScore)
        || storedScore < 0
        || storedScore > std::numeric_limits<int>::max())
    {
        return highScore_;
    }

    // Reject trailing non-whitespace data.
    file >> std::ws;

    if (!file.eof())
    {
        return highScore_;
    }

    highScore_ = static_cast<int>(storedScore);
    return highScore_;
}

bool HighScoreManager::update(int score) noexcept
{
    if (score < 0 || score <= highScore_)
    {
        return false;
    }

    const int previousHighScore = highScore_;
    highScore_ = score;

    if (!save())
    {
        highScore_ = previousHighScore;
        return false;
    }

    return true;
}

int HighScoreManager::highScore() const noexcept
{
    return highScore_;
}

bool HighScoreManager::save() const noexcept
{
    std::ofstream file(filePath_, std::ios::trunc);

    if (!file.is_open())
    {
        return false;
    }

    file << highScore_ << '\n';

    return file.good();
}
} // namespace snakearcade::persistence
