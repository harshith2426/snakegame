#pragma once

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

private:
    void processEvents(sf::RenderWindow& window);
    void update(float elapsedSeconds);
    void render(sf::RenderWindow& window);

    snakearcade::model::Snake snake_;
    snakearcade::model::MovementTimer movementTimer_;
    snakearcade::model::FoodScoreManager foodScoreManager_;
    std::optional<snakearcade::model::GridPosition> food_;
    int score_{0};
    int lastDisplayedScore_{-1};
};
