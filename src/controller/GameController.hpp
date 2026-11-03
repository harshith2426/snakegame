#pragma once

#include "model/CollisionManager.hpp"
#include "model/FoodScoreManager.hpp"
#include "model/MovementTimer.hpp"
#include "model/Snake.hpp"

#include <optional>

namespace sf
{
class RenderWindow;
}

class GameController final
{
public:
    GameController();
    int run();

    bool isGameOver() const noexcept;
    void restart();

private:
    void processEvents(sf::RenderWindow& window);
    void update(float elapsedSeconds);
    void render(sf::RenderWindow& window);

    snakearcade::model::Snake snake_;
    snakearcade::model::MovementTimer movementTimer_;
    snakearcade::model::FoodScoreManager foodScoreManager_;
    snakearcade::model::CollisionManager collisionManager_;
    std::optional<snakearcade::model::GridPosition> food_;
    int score_{0};
    int lastDisplayedScore_{-1};
    bool isGameOver_{false};
    bool lastDisplayedGameOverState_{false};
};
