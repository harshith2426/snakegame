#pragma once

#include "model/CollisionManager.hpp"
#include "model/FoodScoreManager.hpp"
#include "model/GameConfig.hpp"
#include "model/MovementTimer.hpp"
#include "model/Snake.hpp"

#include <optional>

namespace snakearcade::model
{
// Coordinates movement, food, scoring, collision, difficulty timing, pause, and end-of-session rules.
class GameplaySession final
{
public:
    explicit GameplaySession(Difficulty difficulty = Difficulty::Medium);

    // Deterministic setup for tests and controlled sessions. An absent or
    // invalid/occupied food position is replaced with a valid spawn when possible.
    GameplaySession(
        Snake snake,
        std::optional<GridPosition> food,
        unsigned int seed,
        Difficulty difficulty = Difficulty::Medium
    );

    void requestDirection(Direction direction) noexcept;
    void advance(float elapsedSeconds) noexcept;
    void restart();
    void restart(Difficulty difficulty);

    void pause() noexcept;
    void resume() noexcept;
    bool isPaused() const noexcept;

    Difficulty difficulty() const noexcept;

    const Snake& snake() const noexcept;
    std::optional<GridPosition> food() const noexcept;
    int score() const noexcept;
    bool isGameOver() const noexcept;
    bool isWon() const noexcept;
    bool isFinished() const noexcept;

private:
    void spawnFoodOrFinish();

    Snake snake_;
    Difficulty difficulty_;
    MovementTimer movementTimer_;
    FoodScoreManager foodScoreManager_;
    CollisionManager collisionManager_;
    std::optional<GridPosition> food_;
    int score_{0};
    bool isGameOver_{false};
    bool isWon_{false};
    bool isPaused_{false};
};
} // namespace snakearcade::model
