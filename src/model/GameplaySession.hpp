
#pragma once

#include "model/CollisionManager.hpp"
#include "model/FoodScoreManager.hpp"
#include "model/MovementTimer.hpp"
#include "model/Snake.hpp"

#include <optional>

namespace snakearcade::model
{
// Coordinates movement, food, scoring, collision, and end-of-session rules.
class GameplaySession final
{
public:
    GameplaySession();

    // Deterministic setup for tests and controlled sessions.
    GameplaySession(Snake snake, std::optional<GridPosition> food, unsigned int seed);

    void requestDirection(Direction direction) noexcept;
    void advance(float elapsedSeconds) noexcept;
    void restart();

    // Pause and resume functionality
    void pause() noexcept;
    void resume() noexcept;
    bool isPaused() const noexcept;

    const Snake& snake() const noexcept;
    std::optional<GridPosition> food() const noexcept;
    int score() const noexcept;
    bool isGameOver() const noexcept;
    bool isWon() const noexcept;
    bool isFinished() const noexcept;

private:
    void spawnFoodOrFinish();

    Snake snake_;
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
