
#include "controller/GameController.hpp"
#include "model/GameConfig.hpp"

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <string>

namespace
{
using snakearcade::model::Direction;
using snakearcade::config::Difficulty;

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

GameController::GameController()
    : session_(),
      highScoreManager_("highscore.txt"),
      selectedDifficulty_(Difficulty::Medium),
      hasStarted_(false),
      highScoreSavedForCurrentGame_(false),
      lastDisplayedScore_(-1),
      lastDisplayedFinishedState_(false),
      lastDisplayedPausedState_(false)
{
    highScoreManager_.load();

    // Create the file with an initial score of 0 if it does not exist.
    highScoreManager_.save();
}

bool GameController::isGameOver() const noexcept
{
    return session_.isGameOver();
}

void GameController::restart()
{
    session_.restart();

    highScoreSavedForCurrentGame_ = false;

    lastDisplayedScore_ = -1;
    lastDisplayedFinishedState_ = false;
    lastDisplayedPausedState_ = false;
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
            const sf::Keyboard::Key key = event.key.code;

            if (!hasStarted_)
            {
                bool difficultyChanged = true;

                switch (key)
                {
                case sf::Keyboard::Num1:
                    selectedDifficulty_ = Difficulty::Easy;
                    break;

                case sf::Keyboard::Num2:
                    selectedDifficulty_ = Difficulty::Medium;
                    break;

                case sf::Keyboard::Num3:
                    selectedDifficulty_ = Difficulty::Hard;
                    break;

                case sf::Keyboard::Return:
                    session_ = snakearcade::model::GameplaySession(
                        selectedDifficulty_
                    );

                    hasStarted_ = true;
                    highScoreSavedForCurrentGame_ = false;

                    lastDisplayedScore_ = -1;
                    lastDisplayedFinishedState_ = false;
                    lastDisplayedPausedState_ = false;
                    break;

                default:
                    difficultyChanged = false;
                    break;
                }

                if (difficultyChanged && !hasStarted_)
                {
                    window.setTitle(
                        std::string("SnakeArcade - Selected: ")
                        + difficultyName(selectedDifficulty_)
                    );
                }

                continue;
            }

            if (session_.isFinished())
            {
                if (key == sf::Keyboard::R)
                {
                    restart();
                }
            }
            else if (key == sf::Keyboard::P)
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

                if (directionForKey(key, requestedDirection))
                {
                    session_.requestDirection(requestedDirection);
                }
            }
        }
    }
}

void GameController::update(float elapsedSeconds)
{
    if (!hasStarted_)
    {
        return;
    }

    session_.advance(elapsedSeconds);

    // Save the score once when the game finishes.
    if (session_.isFinished() && !highScoreSavedForCurrentGame_)
    {
        highScoreManager_.update(session_.score());

        highScoreSavedForCurrentGame_ = true;
        lastDisplayedScore_ = -1;
    }
}

void GameController::render(sf::RenderWindow& window)
{
    using namespace snakearcade;

    window.clear(sf::Color(18, 24, 32));

    if (!hasStarted_)
    {
        sf::Font font;

        if (!font.loadFromFile("assets/fonts/tuffy.ttf"))
        {
            window.setTitle(
                "SnakeArcade - Font loading failed. Run from project root."
            );
            window.display();
            return;
        }

        sf::Text heading("SNAKE ARCADE", font, 42);
        heading.setFillColor(sf::Color::White);
        heading.setPosition(250.0F, 100.0F);
        window.draw(heading);

        sf::Text instruction("SELECT YOUR DIFFICULTY", font, 25);
        instruction.setFillColor(sf::Color(180, 190, 200));
        instruction.setPosition(250.0F, 190.0F);
        window.draw(instruction);

        const char* options[] = {
            "1. EASY",
            "2. MEDIUM",
            "3. HARD"
        };

        const Difficulty difficulties[] = {
            Difficulty::Easy,
            Difficulty::Medium,
            Difficulty::Hard
        };

        for (int i = 0; i < 3; ++i)
        {
            sf::Text option(options[i], font, 30);

            option.setFillColor(
                selectedDifficulty_ == difficulties[i]
                    ? sf::Color(80, 220, 120)
                    : sf::Color::White
            );

            option.setPosition(310.0F, 270.0F + i * 55.0F);
            window.draw(option);
        }

        sf::Text highScoreText(
            "HIGH SCORE: " + std::to_string(highScoreManager_.highScore()),
            font,
            22
        );

        highScoreText.setFillColor(sf::Color(255, 210, 80));
        highScoreText.setPosition(300.0F, 465.0F);
        window.draw(highScoreText);

        sf::Text startInstruction("Press ENTER to start", font, 22);
        startInstruction.setFillColor(sf::Color(180, 190, 200));
        startInstruction.setPosition(290.0F, 510.0F);
        window.draw(startInstruction);

        window.display();
        return;
    }

    if (session_.score() != lastDisplayedScore_
        || session_.isFinished() != lastDisplayedFinishedState_
        || session_.isPaused() != lastDisplayedPausedState_)
    {
        if (session_.isPaused())
        {
            window.setTitle("SnakeArcade - PAUSED - Press P to resume");
        }
        else if (session_.isWon())
        {
            window.setTitle(
                "SnakeArcade - YOU WIN! Score: "
                + std::to_string(session_.score())
                + " | High Score: "
                + std::to_string(highScoreManager_.highScore())
                + " - Press R to restart"
            );
        }
        else if (session_.isGameOver())
        {
            window.setTitle(
                "SnakeArcade - GAME OVER! Score: "
                + std::to_string(session_.score())
                + " | High Score: "
                + std::to_string(highScoreManager_.highScore())
                + " - Press R to restart"
            );
        }
        else
        {
            window.setTitle(
                std::string("SnakeArcade - ")
                + difficultyName(session_.difficulty())
                + " - Score: "
                + std::to_string(session_.score())
                + " - High Score: "
                + std::to_string(highScoreManager_.highScore())
                + " - P to pause"
            );
        }

        lastDisplayedScore_ = session_.score();
        lastDisplayedFinishedState_ = session_.isFinished();
        lastDisplayedPausedState_ = session_.isPaused();
    }

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
