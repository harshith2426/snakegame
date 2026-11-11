#pragma once

#include "controller/GameStateFlow.hpp"
#include "model/GameplaySession.hpp"
#include "persistence/HighScoreManager.hpp"

#include <string>

// Integrates application state flow, gameplay session rules, difficulty timing,
// and high-score persistence without depending on SFML.
class GameCoordinator final
{
public:
    using State = GameStateFlow::State;
    using Difficulty = GameStateFlow::Difficulty;

    explicit GameCoordinator(std::string highScoreFilePath = "highscore.txt");

    bool selectDifficulty(Difficulty difficulty) noexcept;
    bool startSession();
    bool startSession(Difficulty difficulty);
    bool startCustomSession(snakearcade::model::GameplaySession customSession);

    bool pause() noexcept;
    bool resume() noexcept;
    bool togglePause() noexcept;
    bool restart();
    bool returnToStart();

    void requestDirection(snakearcade::model::Direction direction) noexcept;
    void update(float elapsedSeconds) noexcept;

    State state() const noexcept;
    Difficulty difficulty() const noexcept;
    const snakearcade::model::GameplaySession& session() const noexcept;
    int score() const noexcept;
    int highScore() const noexcept;

    bool isPlaying() const noexcept;
    bool isPaused() const noexcept;
    bool isGameOver() const noexcept;
    bool isWon() const noexcept;

private:
    void syncFinishedSession() noexcept;

    GameStateFlow stateFlow_;
    snakearcade::model::GameplaySession session_;
    snakearcade::persistence::HighScoreManager highScoreManager_;
    bool highScoreSavedForCurrentGame_{false};
};

