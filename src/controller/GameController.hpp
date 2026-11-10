
#pragma once

#include "model/GameplaySession.hpp"

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

    snakearcade::model::GameplaySession session_;

    snakearcade::config::Difficulty selectedDifficulty_{
        snakearcade::config::Difficulty::Medium
    };

    bool hasStarted_{false};

    int lastDisplayedScore_{-1};
    bool lastDisplayedFinishedState_{false};
    bool lastDisplayedPausedState_{false};
};
