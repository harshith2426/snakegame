#pragma once

#include "model/MovementTimer.hpp"
#include "model/Snake.hpp"

namespace sf
{
class RenderWindow;
}

class GameController final
{
public:
    int run();

private:
    void processEvents(sf::RenderWindow& window);
    void update(float elapsedSeconds);
    void render(sf::RenderWindow& window);

    snakearcade::model::Snake snake_;
    snakearcade::model::MovementTimer movementTimer_;
};
