#include "controller/GameController.hpp"

#include <SFML/Graphics.hpp>

#include <cstdlib>

int GameController::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SnakeArcade");
    if (!window.isOpen())
    {
        return EXIT_FAILURE;
    }

    window.setFramerateLimit(60);
    sf::Clock frameClock;

    while (window.isOpen())
    {
        processEvents(window);
        if (!window.isOpen())
        {
            break;
        }

        const float elapsedSeconds = frameClock.restart().asSeconds();
        update(elapsedSeconds);
        render(window);
    }

    return EXIT_SUCCESS;
}

void GameController::processEvents(sf::RenderWindow& window)
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void GameController::update(float elapsedSeconds)
{
    // Gameplay state and model updates are introduced in later sprint tasks.
    (void)elapsedSeconds;
}

void GameController::render(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.display();
}
