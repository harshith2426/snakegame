#include "model/GameplaySession.hpp"

#include <utility>

namespace snakearcade::model
{
GameplaySession::GameplaySession()
    : snake_(),
      movementTimer_(),
      foodScoreManager_(),
      collisionManager_()
{
    spawnFoodOrFinish();
}

GameplaySession::GameplaySession(Snake snake, std::optional<GridPosition> food, unsigned int seed)
    : snake_(std::move(snake)),
      movementTimer_(),
      foodScoreManager_(seed),
      collisionManager_(),
      food_(food)
{
    if (!food_.has_value() || !isWithinBoard(*food_) || snake_.occupies(*food_))
    {
        spawnFoodOrFinish();
    }
}

void GameplaySession::requestDirection(Direction direction) noexcept
{
    if (!isFinished())
    {
        snake_.setDirection(direction);
    }
}

void GameplaySession::advance(float elapsedSeconds) noexcept
{
    if (isFinished())
    {
        return;
    }

    const int movementTicks = movementTimer_.advance(elapsedSeconds);
    for (int tick = 0; tick < movementTicks && !isFinished(); ++tick)
    {
        const GridPosition next = snake_.nextHead();
        const Collision collision = collisionManager_.checkCollision(next, snake_.segments());
        if (collision != Collision::None)
        {
            isGameOver_ = true;
            break;
        }

        snake_.move();
        if (food_.has_value() && foodScoreManager_.isConsumed(snake_.head(), *food_))
        {
            snake_.grow();
            score_ = foodScoreManager_.applyScore(score_);
            spawnFoodOrFinish();
        }
    }
}

void GameplaySession::restart()
{
    snake_.reset();
    movementTimer_.reset();
    score_ = 0;
    isGameOver_ = false;
    isWon_ = false;
    spawnFoodOrFinish();
}

const Snake& GameplaySession::snake() const noexcept
{
    return snake_;
}

std::optional<GridPosition> GameplaySession::food() const noexcept
{
    return food_;
}

int GameplaySession::score() const noexcept
{
    return score_;
}

bool GameplaySession::isGameOver() const noexcept
{
    return isGameOver_;
}

bool GameplaySession::isWon() const noexcept
{
    return isWon_;
}

bool GameplaySession::isFinished() const noexcept
{
    return isGameOver_ || isWon_;
}

void GameplaySession::spawnFoodOrFinish()
{
    food_ = foodScoreManager_.spawnFood(snake_.segments());
    if (!food_.has_value())
    {
        isWon_ = true;
    }
}
} // namespace snakearcade::model
