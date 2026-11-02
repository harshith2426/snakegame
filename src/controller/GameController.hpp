#pragma once

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
};
