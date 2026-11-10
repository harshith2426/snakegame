
#pragma once

#include "model/GameplaySession.hpp"
#include "persistence/HighScoreManager.hpp"

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

    snakearcade::persistence::HighScoreManager highScoreManager_;

    snakearcade::config::Difficulty selectedDifficulty_{
        snakearcade::config::Difficulty::Medium
    };

    bool hasStarted_{false};
    bool highScoreSavedForCurrentGame_{false};

    int lastDisplayedScore_{-1};
    bool lastDisplayedFinishedState_{false};
    bool lastDisplayedPausedState_{false};
};
