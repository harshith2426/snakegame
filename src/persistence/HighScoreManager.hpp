
#pragma once

#include <string>

namespace snakearcade::persistence
{
class HighScoreManager final
{
public:
    explicit HighScoreManager(
        std::string filePath = "highscore.txt"
    );

    int load() noexcept;

    bool update(int score) noexcept;

    int highScore() const noexcept;

    bool save() const noexcept;

private:
    std::string filePath_;
    int highScore_{0};
};
} // namespace snakearcade::persistence
