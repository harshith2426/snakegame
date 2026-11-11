#include "persistence/HighScoreManager.hpp"

#include <cctype>
#include <fstream>
#include <limits>
#include <string>
#include <utility>

namespace snakearcade::persistence
{
namespace
{
bool parseStrictNonNegativeInt(const std::string& text, int& outValue) noexcept
{
    std::size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start])) != 0)
    {
        ++start;
    }

    std::size_t end = text.size();
    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1])) != 0)
    {
        --end;
    }

    if (start >= end)
    {
        return false;
    }

    long long accumulator = 0;
    constexpr long long maxInt = std::numeric_limits<int>::max();

    for (std::size_t index = start; index < end; ++index)
    {
        const unsigned char ch = static_cast<unsigned char>(text[index]);
        if (ch < '0' || ch > '9')
        {
            return false;
        }

        accumulator = accumulator * 10 + (ch - '0');
        if (accumulator > maxInt)
        {
            return false;
        }
    }

    outValue = static_cast<int>(accumulator);
    return true;
}
} // namespace

HighScoreManager::HighScoreManager(std::string filePath)
    : filePath_(std::move(filePath))
{
}

int HighScoreManager::load() noexcept
{
    highScore_ = 0;

    try
    {
        std::ifstream file(filePath_);
        if (!file.is_open())
        {
            return highScore_;
        }

        // Bound read length to prevent reading arbitrarily large malformed files.
        char buffer[128]{};
        file.read(buffer, sizeof(buffer) - 1);
        const std::streamsize bytesRead = file.gcount();

        // If more bytes remain beyond the buffer, verify they are purely whitespace.
        int extraChar = file.peek();
        while (extraChar != std::char_traits<char>::eof())
        {
            if (std::isspace(static_cast<unsigned char>(extraChar)) == 0)
            {
                return highScore_;
            }
            file.get();
            extraChar = file.peek();
        }

        const std::string content(buffer, static_cast<std::size_t>(bytesRead));
        int parsedScore = 0;
        if (parseStrictNonNegativeInt(content, parsedScore))
        {
            highScore_ = parsedScore;
        }
    }
    catch (...)
    {
        highScore_ = 0;
    }

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
    try
    {
        std::ofstream file(filePath_, std::ios::trunc);
        if (!file.is_open())
        {
            return false;
        }

        file << highScore_ << '\n';
        file.flush();

        return file.good();
    }
    catch (...)
    {
        return false;
    }
}
} // namespace snakearcade::persistence
