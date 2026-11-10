
#include "controller/GameController.hpp"
#include "model/GameConfig.hpp"

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <string>

namespace
{
using snakearcade::model::Direction;

bool directionForKey(sf::Keyboard::Key key, Direction& direction)
{
    switch (key)
    {
    case sf::Keyboard::Up:
    case sf::Keyboard::W:
        direction = Direction::Up;
        return true;

    case sf::Keyboard::Down:
    case sf::Keyboard::S:
        direction = Direction::Down;
        return true;

    case sf::Keyboard::Left:
    case sf::Keyboard::A:
        direction = Direction::Left;
        return true;

    case sf::Keyboard::Right:
    case sf::Keyboard::D:
        direction = Direction::Right;
        return true;

    default:
        return false;
    }
}
} // namespace

GameController::GameController()
    : session_(),
      lastDisplayedScore_(-1),
      lastDisplayedFinishedState_(false),
      lastDisplayedPausedState_(false)
{
}

bool GameController::isGameOver() const noexcept
{
    return session_.isGameOver();
}

void GameController::restart()
{
    session_.restart();

    lastDisplayedScore_ = -1;
    lastDisplayedFinishedState_ = false;
    lastDisplayedPausedState_ = false;
}

int GameController::run()
{
    sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "SnakeArcade - Arrow keys or WASD to move"
    );

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
        else if (event.type == sf::Event::KeyPressed)
        {
            if (session_.isFinished())
            {
                if (event.key.code == sf::Keyboard::R)
                {
                    restart();
                }
            }
            else if (event.key.code == sf::Keyboard::P)
            {
                if (session_.isPaused())
                {
                    session_.resume();
                }
                else
                {
                    session_.pause();
                }
            }
            else if (!session_.isPaused())
            {
                Direction requestedDirection{};

                if (directionForKey(event.key.code, requestedDirection))
                {
                    session_.requestDirection(requestedDirection);
                }
            }
        }
    }
}

void GameController::update(float elapsedSeconds)
{
    session_.advance(elapsedSeconds);
}

void GameController::render(sf::RenderWindow& window)
{
    using namespace snakearcade;

    if (session_.score() != lastDisplayedScore_
        || session_.isFinished() != lastDisplayedFinishedState_
        || session_.isPaused() != lastDisplayedPausedState_)
    {
        if (session_.isPaused())
        {
            window.setTitle(
                "SnakeArcade - PAUSED - Press P to resume"
            );
        }
        else if (session_.isWon())
        {
            window.setTitle(
                "SnakeArcade - YOU WIN! Final Score: "
                + std::to_string(session_.score())
                + " - Press R to restart"
            );
        }
        else if (session_.isGameOver())
        {
            window.setTitle(
                "SnakeArcade - GAME OVER! Final Score: "
                + std::to_string(session_.score())
                + " - Press R to restart"
            );
        }
        else
        {
            window.setTitle(
                "SnakeArcade - Score: "
                + std::to_string(session_.score())
                + " - P to pause"
            );
        }

        lastDisplayedScore_ = session_.score();
        lastDisplayedFinishedState_ = session_.isFinished();
        lastDisplayedPausedState_ = session_.isPaused();
    }

    window.clear(sf::Color(18, 24, 32));

    if (session_.food().has_value() && !session_.isFinished())
    {
        sf::RectangleShape foodCell(sf::Vector2f(
            static_cast<float>(config::CellSizePixels - 2),
            static_cast<float>(config::CellSizePixels - 2)
        ));

        foodCell.setPosition(
            static_cast<float>(session_.food()->x * config::CellSizePixels + 1),
            static_cast<float>(session_.food()->y * config::CellSizePixels + 1)
        );

        foodCell.setFillColor(sf::Color(230, 60, 60));
        window.draw(foodCell);
    }

    sf::RectangleShape cell(sf::Vector2f(
        static_cast<float>(config::CellSizePixels - 2),
        static_cast<float>(config::CellSizePixels - 2)
    ));

    for (const model::GridPosition segment : session_.snake().segments())
    {
        cell.setPosition(
            static_cast<float>(segment.x * config::CellSizePixels + 1),
            static_cast<float>(segment.y * config::CellSizePixels + 1)
        );

        if (session_.isFinished())
        {
            cell.setFillColor(
                segment == session_.snake().head()
                    ? sf::Color(230, 80, 80)
                    : sf::Color(150, 60, 60)
            );
        }
        else
        {
            cell.setFillColor(
                segment == session_.snake().head()
                    ? sf::Color(80, 220, 120)
                    : sf::Color(40, 160, 90)
            );
        }

        window.draw(cell);
    }

    window.display();
}
