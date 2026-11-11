#pragma once

#include "model/GameTypes.hpp"

// Owns the application's high-level state and selected difficulty.
// This class deliberately has no SFML dependency so it can be unit-tested in isolation.
class GameStateFlow final
{
public:
    using State = snakearcade::model::GameState;
    using Difficulty = snakearcade::model::Difficulty;

    State state() const noexcept;
    Difficulty difficulty() const noexcept;

    // Select difficulty while on the Start screen; rejected during active or finished sessions.
    bool selectDifficulty(Difficulty difficulty) noexcept;

    // Start a new session from Start, or restart after GameOver.
    bool startSession(Difficulty difficulty) noexcept;
    bool pause() noexcept;          // Playing -> Paused
    bool resume() noexcept;         // Paused -> Playing
    bool finishSession() noexcept;  // Playing -> GameOver
    bool returnToStart() noexcept;  // GameOver -> Start

    bool isPlaying() const noexcept;

private:
    State state_{State::Start};
    Difficulty difficulty_{Difficulty::Medium};
};
