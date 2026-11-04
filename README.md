# SnakeArcade

SnakeArcade is a single-player desktop Snake game planned in C++17 with SFML 2.6.

## Build the project on Windows

Install the required tools and follow the shared setup instructions in [Required installations](docs/required-installations.md). From an MSYS2 UCRT64 terminal at the repository root, run:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

The first configure fetches SFML 2.6.2 automatically. The game opens an 800×600 window with a 40×30 cell board. Use the arrow keys or WASD to steer, collect food to grow and score, and avoid walls and the snake's body. Press **R** after game over to restart.

## Build and run automated tests

GoogleTest is fetched automatically by CMake when testing is enabled. From the MSYS2 UCRT64 terminal at the repository root, configure and build the game and test target, then run the tests with CTest:

```bash
cmake -S . -B build -G "MinGW Makefiles" -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

GitHub Actions runs the same configure, build, and test steps on every push and pull request. Automated tests cover the snake model, movement timing, food/scoring, collision rules, and integrated gameplay session behavior.

## Day 6 snake model

The snake is represented as head-first integer grid coordinates, with board dimensions and starting length in `src/model/GameConfig.hpp`. See [Day 6 snake model](docs/day6-snake-model.md) for the starting position and model contract.

Run the built game with:

```bash
./build/snakearcade.exe
```

## Day 7 directional movement

Use the arrow keys or WASD to steer the snake. Movement advances one grid cell
every 0.15 seconds, independent of the rendering frame rate. Immediate reverse
turns are ignored. See [Day 7 directional movement](docs/day7-directional-movement.md).

## Day 10 integrated gameplay

See [Day 10 integrated gameplay](docs/day10-core-gameplay.md) for the integrated session rules and verification scope.

## Shared project references

The original v1.0 documents remain the team's requirements, architecture, and test-plan baseline. The Day 2 architecture notes record the current Sai/Rohil swap from the sprint board without altering those PDFs.

- [Project synopsis v1.0](SnakeArcade_Synopsis_TEAM1_SECTIONG.pdf)
- [Combined SRS, SAD, and Software Test Plan v1.0](SnakeArcade_Combined_SRS_SAD_TestPlan_SECTIONG_TEAM1.pdf)
- [Day 2 architecture and module interfaces](docs/architecture-day2.md)
- [Required installations and setup instructions](docs/required-installations.md)
