# SnakeArcade

SnakeArcade is a single-player desktop Snake game planned in C++17 with SFML 2.6.

## Build the starter project on Windows

Install the required tools and follow the shared setup instructions in [Required installations](docs/required-installations.md). From an MSYS2 UCRT64 terminal at the repository root, run:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

The first configure fetches SFML 2.6.2 automatically. The starter opens a blank 800×600 SFML window; gameplay features are implemented in later sprint tasks.

Run the built starter with:

```bash
./build/snakearcade.exe
```

## Shared project references

The original v1.0 documents remain the team's requirements, architecture, and test-plan baseline. The Day 2 architecture notes record the current Sai/Rohil swap from the sprint board without altering those PDFs.

- [Project synopsis v1.0](SnakeArcade_Synopsis_TEAM1_SECTIONG.pdf)
- [Combined SRS, SAD, and Software Test Plan v1.0](SnakeArcade_Combined_SRS_SAD_TestPlan_SECTIONG_TEAM1.pdf)
- [Day 2 architecture and module interfaces](docs/architecture-day2.md)
- [Required installations and setup instructions](docs/required-installations.md)
