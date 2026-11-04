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
    case sf::Keyboard::W: direction = Direction::Up; return true;
    case sf::Keyboard::Down:
    case sf::Keyboard::S: direction = Direction::Down; return true;
    case sf::Keyboard::Left:
    case sf::Keyboard::A: direction = Direction::Left; return true;
    case sf::Keyboard::Right:
    case sf::Keyboard::D: direction = Direction::Right; return true;
    default: return false;
    }
}
} // namespace

GameController::GameController()
    : food_(foodScoreManager_.spawnFood(snake_.segments())),
      score_(0),
      lastDisplayedScore_(-1)
{
}

int GameController::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SnakeArcade - Arrow keys or WASD to move");
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
            Direction requestedDirection{};
            if (directionForKey(event.key.code, requestedDirection))
            {
                snake_.setDirection(requestedDirection);
            }
        }
    }
}

void GameController::update(float elapsedSeconds)
{
    const int movementTicks = movementTimer_.advance(elapsedSeconds);
    for (int tick = 0; tick < movementTicks; ++tick)
    {
        snake_.move();
        if (food_.has_value() && foodScoreManager_.isConsumed(snake_.head(), *food_))
        {
            snake_.grow();
            score_ = foodScoreManager_.applyScore(score_);
            food_ = foodScoreManager_.spawnFood(snake_.segments());
        }
    }
}

void GameController::render(sf::RenderWindow& window)
{
    using namespace snakearcade;

    if (score_ != lastDisplayedScore_)
    {
        window.setTitle("SnakeArcade - Score: " + std::to_string(score_) + " - Arrow keys or WASD to move");
        lastDisplayedScore_ = score_;
    }

    window.clear(sf::Color(18, 24, 32));

    if (food_.has_value())
    {
        sf::RectangleShape foodCell(sf::Vector2f(
            static_cast<float>(config::CellSizePixels - 2),
            static_cast<float>(config::CellSizePixels - 2)));
        foodCell.setPosition(
            static_cast<float>(food_->x * config::CellSizePixels + 1),
            static_cast<float>(food_->y * config::CellSizePixels + 1));
        foodCell.setFillColor(sf::Color(230, 60, 60));
        window.draw(foodCell);
    }

    sf::RectangleShape cell(sf::Vector2f(
        static_cast<float>(config::CellSizePixels - 2),
        static_cast<float>(config::CellSizePixels - 2)));
    for (const model::GridPosition segment : snake_.segments())
    {
        cell.setPosition(static_cast<float>(segment.x * config::CellSizePixels + 1),
                         static_cast<float>(segment.y * config::CellSizePixels + 1));
        cell.setFillColor(segment == snake_.head() ? sf::Color(80, 220, 120)
                                                   : sf::Color(40, 160, 90));
        window.draw(cell);
    }
    window.display();
}
