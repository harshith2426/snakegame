#pragma once

#include "controller/GameCoordinator.hpp"

#include <SFML/Graphics/Font.hpp>
#include <string>

namespace sf
{
class RenderWindow;
}

class GameController final
{
public:
    explicit GameController(std::string highScoreFilePath = "highscore.txt");
    int run();

    bool isGameOver() const noexcept;
    void restart();

private:
    void loadFont();
    void processEvents(sf::RenderWindow& window);
    void update(float elapsedSeconds);
    void render(sf::RenderWindow& window);
    void updateWindowTitle(sf::RenderWindow& window);
    void renderStartScreen(sf::RenderWindow& window) const;
    void renderBoard(sf::RenderWindow& window) const;
    void renderOverlay(sf::RenderWindow& window) const;
    void renderHud(sf::RenderWindow& window) const;

    GameCoordinator coordinator_;
    sf::Font font_;
    bool fontLoaded_{false};

    GameCoordinator::State lastDisplayedState_{GameCoordinator::State::GameOver};
    GameCoordinator::Difficulty lastDisplayedDifficulty_{GameCoordinator::Difficulty::Easy};
    int lastDisplayedScore_{-1};
    int lastDisplayedHighScore_{-1};
    bool lastDisplayedWonState_{false};
};
