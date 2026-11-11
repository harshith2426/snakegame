#include "controller/GameController.hpp"
#include "model/GameConfig.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <cstdlib>
#include <string>
#include <utility>

namespace
{
using snakearcade::model::Difficulty;
using snakearcade::model::Direction;
using snakearcade::model::GameState;

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

const char* difficultyName(Difficulty difficulty)
{
    switch (difficulty)
    {
    case Difficulty::Easy:
        return "Easy";
    case Difficulty::Medium:
        return "Medium";
    case Difficulty::Hard:
        return "Hard";
    }

    return "Medium";
}
} // namespace

GameController::GameController(std::string highScoreFilePath)
    : coordinator_(std::move(highScoreFilePath))
{
}

bool GameController::isGameOver() const noexcept
{
    return coordinator_.isGameOver();
}

void GameController::restart()
{
    coordinator_.restart();
}

void GameController::loadFont()
{
    constexpr std::array<const char*, 2> candidates = {
        "assets/fonts/tuffy.ttf",
        "../assets/fonts/tuffy.ttf"
    };

    for (const char* path : candidates)
    {
        if (font_.loadFromFile(path))
        {
            fontLoaded_ = true;
            return;
        }
    }

    fontLoaded_ = false;
}

int GameController::run()
{
    sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "SnakeArcade - Select Difficulty"
    );

    if (!window.isOpen())
    {
        return EXIT_FAILURE;
    }

    loadFont();
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
            continue;
        }

        if (event.type != sf::Event::KeyPressed)
        {
            continue;
        }

        const sf::Keyboard::Key key = event.key.code;
        const GameState currentState = coordinator_.state();

        if (currentState == GameState::Start)
        {
            switch (key)
            {
            case sf::Keyboard::Num1:
                coordinator_.selectDifficulty(Difficulty::Easy);
                break;
            case sf::Keyboard::Num2:
                coordinator_.selectDifficulty(Difficulty::Medium);
                break;
            case sf::Keyboard::Num3:
                coordinator_.selectDifficulty(Difficulty::Hard);
                break;
            case sf::Keyboard::Return:
                coordinator_.startSession();
                break;
            default:
                break;
            }
        }
        else if (currentState == GameState::Playing)
        {
            if (key == sf::Keyboard::P)
            {
                coordinator_.pause();
            }
            else
            {
                Direction requestedDirection{};
                if (directionForKey(key, requestedDirection))
                {
                    coordinator_.requestDirection(requestedDirection);
                }
            }
        }
        else if (currentState == GameState::Paused)
        {
            if (key == sf::Keyboard::P)
            {
                coordinator_.resume();
            }
        }
        else if (currentState == GameState::GameOver)
        {
            if (key == sf::Keyboard::R)
            {
                coordinator_.restart();
            }
            else if (key == sf::Keyboard::Escape || key == sf::Keyboard::M)
            {
                coordinator_.returnToStart();
            }
        }
    }
}

void GameController::update(float elapsedSeconds)
{
    coordinator_.update(elapsedSeconds);
}

void GameController::updateWindowTitle(sf::RenderWindow& window)
{
    const GameState state = coordinator_.state();
    const Difficulty difficulty = coordinator_.difficulty();
    const int score = coordinator_.score();
    const int highScore = coordinator_.highScore();
    const bool won = coordinator_.isWon();

    if (state == lastDisplayedState_
        && difficulty == lastDisplayedDifficulty_
        && score == lastDisplayedScore_
        && highScore == lastDisplayedHighScore_
        && won == lastDisplayedWonState_)
    {
        return;
    }

    switch (state)
    {
    case GameState::Start:
        window.setTitle(
            std::string("SnakeArcade - Difficulty: ")
            + difficultyName(difficulty)
            + " | High Score: "
            + std::to_string(highScore)
            + " | Press 1/2/3, ENTER to start"
        );
        break;

    case GameState::Playing:
        window.setTitle(
            std::string("SnakeArcade - ")
            + difficultyName(difficulty)
            + " | Score: "
            + std::to_string(score)
            + " | High Score: "
            + std::to_string(highScore)
            + " | P to pause"
        );
        break;

    case GameState::Paused:
        window.setTitle(
            std::string("SnakeArcade - PAUSED | Score: ")
            + std::to_string(score)
            + " | Press P to resume"
        );
        break;

    case GameState::GameOver:
        window.setTitle(
            std::string(won ? "SnakeArcade - YOU WIN! Score: " : "SnakeArcade - GAME OVER! Score: ")
            + std::to_string(score)
            + " | High Score: "
            + std::to_string(highScore)
            + " | R to restart, ESC for menu"
        );
        break;
    }

    lastDisplayedState_ = state;
    lastDisplayedDifficulty_ = difficulty;
    lastDisplayedScore_ = score;
    lastDisplayedHighScore_ = highScore;
    lastDisplayedWonState_ = won;
}

void GameController::renderStartScreen(sf::RenderWindow& window) const
{
    constexpr std::array<const char*, 3> labels = {
        "1. EASY (Slow)",
        "2. MEDIUM (Normal)",
        "3. HARD (Fast)"
    };
    constexpr std::array<Difficulty, 3> difficulties = {
        Difficulty::Easy,
        Difficulty::Medium,
        Difficulty::Hard
    };

    if (!fontLoaded_)
    {
        // Minimal visual indicator when font asset is unavailable.
        for (int i = 0; i < 3; ++i)
        {
            sf::RectangleShape bar(sf::Vector2f(220.0F, 36.0F));
            bar.setPosition(290.0F, 260.0F + static_cast<float>(i) * 55.0F);
            bar.setFillColor(
                coordinator_.difficulty() == difficulties[i]
                    ? sf::Color(80, 220, 120)
                    : sf::Color(60, 72, 88)
            );
            window.draw(bar);
        }
        return;
    }

    sf::Text heading("SNAKE ARCADE", font_, 42);
    heading.setFillColor(sf::Color::White);
    heading.setPosition(245.0F, 95.0F);
    window.draw(heading);

    sf::Text instruction("SELECT DIFFICULTY", font_, 24);
    instruction.setFillColor(sf::Color(180, 190, 200));
    instruction.setPosition(280.0F, 185.0F);
    window.draw(instruction);

    for (int i = 0; i < 3; ++i)
    {
        sf::Text option(labels[i], font_, 28);
        option.setFillColor(
            coordinator_.difficulty() == difficulties[i]
                ? sf::Color(80, 220, 120)
                : sf::Color::White
        );
        option.setPosition(270.0F, 265.0F + static_cast<float>(i) * 55.0F);
        window.draw(option);
    }

    sf::Text highScoreText(
        "HIGH SCORE: " + std::to_string(coordinator_.highScore()),
        font_,
        22
    );
    highScoreText.setFillColor(sf::Color(255, 210, 80));
    highScoreText.setPosition(305.0F, 460.0F);
    window.draw(highScoreText);

    sf::Text startInstruction("Press ENTER to start", font_, 22);
    startInstruction.setFillColor(sf::Color(180, 190, 200));
    startInstruction.setPosition(290.0F, 505.0F);
    window.draw(startInstruction);
}

void GameController::renderBoard(sf::RenderWindow& window) const
{
    using namespace snakearcade;

    const auto& session = coordinator_.session();
    const GameState state = coordinator_.state();

    if (session.food().has_value() && !session.isFinished())
    {
        sf::RectangleShape foodCell(sf::Vector2f(
            static_cast<float>(config::CellSizePixels - 2),
            static_cast<float>(config::CellSizePixels - 2)
        ));
        foodCell.setPosition(
            static_cast<float>(session.food()->x * config::CellSizePixels + 1),
            static_cast<float>(session.food()->y * config::CellSizePixels + 1)
        );
        foodCell.setFillColor(sf::Color(230, 60, 60));
        window.draw(foodCell);
    }

    sf::RectangleShape cell(sf::Vector2f(
        static_cast<float>(config::CellSizePixels - 2),
        static_cast<float>(config::CellSizePixels - 2)
    ));

    for (const model::GridPosition segment : session.snake().segments())
    {
        cell.setPosition(
            static_cast<float>(segment.x * config::CellSizePixels + 1),
            static_cast<float>(segment.y * config::CellSizePixels + 1)
        );

        const bool isHead = segment == session.snake().head();
        if (state == GameState::GameOver)
        {
            cell.setFillColor(isHead ? sf::Color(230, 80, 80) : sf::Color(150, 60, 60));
        }
        else if (state == GameState::Paused)
        {
            cell.setFillColor(isHead ? sf::Color(240, 200, 80) : sf::Color(180, 140, 50));
        }
        else
        {
            cell.setFillColor(isHead ? sf::Color(80, 220, 120) : sf::Color(40, 160, 90));
        }

        window.draw(cell);
    }
}

void GameController::renderOverlay(sf::RenderWindow& window) const
{
    const GameState state = coordinator_.state();
    if (!fontLoaded_ || (state != GameState::Paused && state != GameState::GameOver))
    {
        return;
    }

    sf::RectangleShape backdrop(sf::Vector2f(800.0F, 600.0F));
    backdrop.setFillColor(sf::Color(10, 14, 20, 150));
    window.draw(backdrop);

    if (state == GameState::Paused)
    {
        sf::Text pausedText("PAUSED", font_, 44);
        pausedText.setFillColor(sf::Color(240, 200, 80));
        pausedText.setPosition(315.0F, 240.0F);
        window.draw(pausedText);

        sf::Text resumeText("Press P to resume", font_, 24);
        resumeText.setFillColor(sf::Color::White);
        resumeText.setPosition(295.0F, 305.0F);
        window.draw(resumeText);
        return;
    }

    const bool won = coordinator_.isWon();
    sf::Text statusText(won ? "YOU WIN!" : "GAME OVER", font_, 44);
    statusText.setFillColor(won ? sf::Color(80, 220, 120) : sf::Color(230, 80, 80));
    statusText.setPosition(won ? 295.0F : 270.0F, 200.0F);
    window.draw(statusText);

    sf::Text scoreText(
        "Score: " + std::to_string(coordinator_.score())
            + "   |   High Score: " + std::to_string(coordinator_.highScore()),
        font_,
        24
    );
    scoreText.setFillColor(sf::Color(255, 210, 80));
    scoreText.setPosition(235.0F, 270.0F);
    window.draw(scoreText);

    sf::Text actionText("Press R to restart  |  ESC for menu", font_, 22);
    actionText.setFillColor(sf::Color::White);
    actionText.setPosition(215.0F, 325.0F);
    window.draw(actionText);
}

void GameController::render(sf::RenderWindow& window)
{
    updateWindowTitle(window);
    window.clear(sf::Color(18, 24, 32));

    if (coordinator_.state() == GameState::Start)
    {
        renderStartScreen(window);
    }
    else
    {
        renderBoard(window);
        renderOverlay(window);
    }

    window.display();
}
