#include "model/GameplaySession.hpp"

#include <utility>

namespace snakearcade::model
{
namespace
{
GridPosition stepPosition(GridPosition origin, Direction direction) noexcept
{
    switch (direction)
    {
    case Direction::Up:
        --origin.y;
        break;
    case Direction::Down:
        ++origin.y;
        break;
    case Direction::Left:
        --origin.x;
        break;
    case Direction::Right:
        ++origin.x;
        break;
    }
    return origin;
}
} // namespace

GameplaySession::GameplaySession(Difficulty difficulty)
    : snake_(),
      difficulty_(difficulty),
      movementTimer_(config::movementInterval(difficulty)),
      foodScoreManager_(),
      collisionManager_()
{
    spawnFoodOrFinish();
}

GameplaySession::GameplaySession(
    Snake snake,
    std::optional<GridPosition> food,
    unsigned int seed,
    Difficulty difficulty
)
    : snake_(std::move(snake)),
      difficulty_(difficulty),
      movementTimer_(config::movementInterval(difficulty)),
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
    if (isFinished() || isPaused_)
    {
        return;
    }

    const auto& segments = snake_.segments();
    if (segments.size() >= 2 && stepPosition(snake_.head(), direction) == segments[1])
    {
        return;
    }

    snake_.setDirection(direction);
}

void GameplaySession::advance(float elapsedSeconds) noexcept
{
    if (isFinished() || isPaused_)
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

void GameplaySession::pause() noexcept
{
    if (!isFinished())
    {
        isPaused_ = true;
    }
}

void GameplaySession::resume() noexcept
{
    if (!isFinished())
    {
        isPaused_ = false;
    }
}

bool GameplaySession::isPaused() const noexcept
{
    return isPaused_;
}

Difficulty GameplaySession::difficulty() const noexcept
{
    return difficulty_;
}

void GameplaySession::restart()
{
    snake_.reset();
    movementTimer_.reset();
    score_ = 0;
    isGameOver_ = false;
    isWon_ = false;
    isPaused_ = false;

    spawnFoodOrFinish();
}

void GameplaySession::restart(Difficulty difficulty)
{
    difficulty_ = difficulty;
    movementTimer_ = MovementTimer(config::movementInterval(difficulty));
    restart();
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
