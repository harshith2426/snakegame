#include "controller/GameCoordinator.hpp"

#include <utility>

GameCoordinator::GameCoordinator(std::string highScoreFilePath)
    : stateFlow_(),
      session_(stateFlow_.difficulty()),
      highScoreManager_(std::move(highScoreFilePath)),
      highScoreSavedForCurrentGame_(false)
{
    highScoreManager_.load();
}

bool GameCoordinator::selectDifficulty(Difficulty difficulty) noexcept
{
    return stateFlow_.selectDifficulty(difficulty);
}

bool GameCoordinator::startSession()
{
    return startSession(stateFlow_.difficulty());
}

bool GameCoordinator::startSession(Difficulty difficulty)
{
    if (!stateFlow_.startSession(difficulty))
    {
        return false;
    }

    session_.restart(difficulty);
    highScoreSavedForCurrentGame_ = false;
    return true;
}

bool GameCoordinator::startCustomSession(snakearcade::model::GameplaySession customSession)
{
    if (!stateFlow_.startSession(customSession.difficulty()))
    {
        return false;
    }

    session_ = std::move(customSession);
    highScoreSavedForCurrentGame_ = false;
    syncFinishedSession();
    return true;
}

bool GameCoordinator::pause() noexcept
{
    if (!stateFlow_.pause())
    {
        return false;
    }

    session_.pause();
    return true;
}

bool GameCoordinator::resume() noexcept
{
    if (!stateFlow_.resume())
    {
        return false;
    }

    session_.resume();
    return true;
}

bool GameCoordinator::togglePause() noexcept
{
    if (stateFlow_.state() == State::Playing)
    {
        return pause();
    }
    if (stateFlow_.state() == State::Paused)
    {
        return resume();
    }
    return false;
}

bool GameCoordinator::restart()
{
    if (stateFlow_.state() != State::GameOver)
    {
        return false;
    }

    return startSession(stateFlow_.difficulty());
}

bool GameCoordinator::returnToStart()
{
    if (!stateFlow_.returnToStart())
    {
        return false;
    }

    session_.restart(stateFlow_.difficulty());
    highScoreSavedForCurrentGame_ = false;
    return true;
}

void GameCoordinator::requestDirection(snakearcade::model::Direction direction) noexcept
{
    if (stateFlow_.isPlaying())
    {
        session_.requestDirection(direction);
    }
}

void GameCoordinator::update(float elapsedSeconds) noexcept
{
    if (!stateFlow_.isPlaying())
    {
        return;
    }

    session_.advance(elapsedSeconds);
    syncFinishedSession();
}

void GameCoordinator::syncFinishedSession() noexcept
{
    if (!session_.isFinished())
    {
        return;
    }

    stateFlow_.finishSession();

    if (!highScoreSavedForCurrentGame_)
    {
        highScoreManager_.update(session_.score());
        highScoreSavedForCurrentGame_ = true;
    }
}

GameCoordinator::State GameCoordinator::state() const noexcept
{
    return stateFlow_.state();
}

GameCoordinator::Difficulty GameCoordinator::difficulty() const noexcept
{
    return stateFlow_.difficulty();
}

const snakearcade::model::GameplaySession& GameCoordinator::session() const noexcept
{
    return session_;
}

int GameCoordinator::score() const noexcept
{
    return session_.score();
}

int GameCoordinator::highScore() const noexcept
{
    return highScoreManager_.highScore();
}

bool GameCoordinator::isPlaying() const noexcept
{
    return stateFlow_.isPlaying();
}

bool GameCoordinator::isPaused() const noexcept
{
    return stateFlow_.state() == State::Paused;
}

bool GameCoordinator::isGameOver() const noexcept
{
    return stateFlow_.state() == State::GameOver;
}

bool GameCoordinator::isWon() const noexcept
{
    return session_.isWon();
}

