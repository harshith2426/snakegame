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
      stateFlow_()
{
}

bool GameController::isGameOver() const noexcept
{
    return stateFlow_.state() == GameStateFlow::State::GameOver;
}

void GameController::restart()
{
    session_.restart();

    if (stateFlow_.state() == GameStateFlow::State::GameOver)
    {
        stateFlow_.startSession(stateFlow_.difficulty());
    }
}

int GameController::run()
{
    sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "SnakeArcade - Press ENTER to start"
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
            const auto currentState = stateFlow_.state();

            // Start the game
            if (currentState == GameStateFlow::State::Start)
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    stateFlow_.startSession(stateFlow_.difficulty());
                }
            }

            // Handle gameplay and pause
            else if (currentState == GameStateFlow::State::Playing)
            {
                if (event.key.code == sf::Keyboard::P)
                {
                    stateFlow_.pause();
                }
                else
                {
                    Direction requestedDirection{};

                    if (directionForKey(event.key.code, requestedDirection))
                    {
                        session_.requestDirection(requestedDirection);
                    }
                }
            }

            // Resume the game
            else if (currentState == GameStateFlow::State::Paused)
            {
                if (event.key.code == sf::Keyboard::P)
                {
                    stateFlow_.resume();
                }
            }

            // Restart after game over
            else if (currentState == GameStateFlow::State::GameOver)
            {
                if (event.key.code == sf::Keyboard::R)
                {
                    restart();
                }
            }
        }
    }
}

void GameController::update(float elapsedSeconds)
{
    // Only update gameplay when the state is Playing.
    if (stateFlow_.isPlaying())
    {
        session_.advance(elapsedSeconds);

        // Detect game over or winning.
        if (session_.isFinished())
        {
            stateFlow_.finishSession();
        }
    }
}

void GameController::render(sf::RenderWindow& window)
{
    using namespace snakearcade;

    const auto currentState = stateFlow_.state();

    // Update the window title according to the current state.
    switch (currentState)
    {
    case GameStateFlow::State::Start:
        window.setTitle("SnakeArcade - Press ENTER to start");
        break;

    case GameStateFlow::State::Playing:
        window.setTitle(
            "SnakeArcade - Score: " + std::to_string(session_.score()) +
            " - Press P to pause"
        );
        break;

    case GameStateFlow::State::Paused:
        window.setTitle(
            "SnakeArcade - PAUSED - Press P to resume"
        );
        break;

    case GameStateFlow::State::GameOver:
        if (session_.isWon())
        {
            window.setTitle(
                "SnakeArcade - YOU WIN! Final Score: " +
                std::to_string(session_.score()) +
                " - Press R to restart"
            );
        }
        else
        {
            window.setTitle(
                "SnakeArcade - GAME OVER! Final Score: " +
                std::to_string(session_.score()) +
                " - Press R to restart"
            );
        }
        break;
    }

    window.clear(sf::Color(18, 24, 32));

    // Draw food.
    if (session_.food().has_value() && !session_.isFinished())
    {
        sf::RectangleShape foodCell(sf::Vector2f(
            static_cast<float>(config::CellSizePixels - 2),
            static_cast<float>(config::CellSizePixels - 2)
        ));

        foodCell.setPosition(
            static_cast<float>(
                session_.food()->x * config::CellSizePixels + 1
            ),
            static_cast<float>(
                session_.food()->y * config::CellSizePixels + 1
            )
        );

        foodCell.setFillColor(sf::Color(230, 60, 60));
        window.draw(foodCell);
    }

    // Draw snake.
    sf::RectangleShape cell(sf::Vector2f(
        static_cast<float>(config::CellSizePixels - 2),
        static_cast<float>(config::CellSizePixels - 2)
    ));

    for (const model::GridPosition segment : session_.snake().segments())
    {
        cell.setPosition(
            static_cast<float>(
                segment.x * config::CellSizePixels + 1
            ),
            static_cast<float>(
                segment.y * config::CellSizePixels + 1
            )
        );

        if (currentState == GameStateFlow::State::GameOver)
        {
            cell.setFillColor(
                segment == session_.snake().head()
                    ? sf::Color(230, 80, 80)
                    : sf::Color(150, 60, 60)
            );
        }
        else if (currentState == GameStateFlow::State::Paused)
        {
            cell.setFillColor(
                segment == session_.snake().head()
                    ? sf::Color(240, 200, 80)
                    : sf::Color(180, 140, 50)
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