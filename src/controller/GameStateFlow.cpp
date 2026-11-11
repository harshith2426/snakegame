#include "controller/GameStateFlow.hpp"

GameStateFlow::State GameStateFlow::state() const noexcept
{
    return state_;
}

GameStateFlow::Difficulty GameStateFlow::difficulty() const noexcept
{
    return difficulty_;
}

bool GameStateFlow::startSession(Difficulty difficulty) noexcept
{
    if (state_ != State::Start && state_ != State::GameOver)
    {
        return false;
    }

    difficulty_ = difficulty;
    state_ = State::Playing;
    return true;
}

bool GameStateFlow::pause() noexcept
{
    if (state_ != State::Playing)
    {
        return false;
    }

    state_ = State::Paused;
    return true;
}

bool GameStateFlow::resume() noexcept
{
    if (state_ != State::Paused)
    {
        return false;
    }

    state_ = State::Playing;
    return true;
}

bool GameStateFlow::finishSession() noexcept
{
    if (state_ != State::Playing)
    {
        return false;
    }

    state_ = State::GameOver;
    return true;
}

bool GameStateFlow::returnToStart() noexcept
{
    if (state_ != State::GameOver)
    {
        return false;
    }

    state_ = State::Start;
    return true;
}

bool GameStateFlow::isPlaying() const noexcept
{
    return state_ == State::Playing;
}
