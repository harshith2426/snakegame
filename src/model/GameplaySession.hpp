
#pragma once

#include "model/CollisionManager.hpp"
#include "model/FoodScoreManager.hpp"
#include "model/MovementTimer.hpp"
#include "model/Snake.hpp"
#include "model/GameConfig.hpp"

#include <optional>

namespace snakearcade::model
{
class GameplaySession final
{
public:
    explicit GameplaySession(config::Difficulty difficulty = config::Difficulty::Medium);

    GameplaySession(
        Snake snake,
        std::optional<GridPosition> food,
        unsigned int seed,
        config::Difficulty difficulty = config::Difficulty::Medium
    );

    void requestDirection(Direction direction) noexcept;
    void advance(float elapsedSeconds) noexcept;
    void restart();

    void pause() noexcept;
    void resume() noexcept;
    bool isPaused() const noexcept;

    config::Difficulty difficulty() const noexcept;

    const Snake& snake() const noexcept;
    std::optional<GridPosition> food() const noexcept;
    int score() const noexcept;
    bool isGameOver() const noexcept;
    bool isWon() const noexcept;
    bool isFinished() const noexcept;

private:
    void spawnFoodOrFinish();

    Snake snake_;
    config::Difficulty difficulty_;
    MovementTimer movementTimer_;
    FoodScoreManager foodScoreManager_;
    CollisionManager collisionManager_;
    std::optional<GridPosition> food_;
    int score_{0};
    bool isGameOver_{false};
    bool isWon_{false};
    bool isPaused_{false};
};
}
