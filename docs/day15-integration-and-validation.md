# Day 15 — State Management, Pause/Restart, Difficulty, and High-Score Integration

**Owners:** All three team members (Rohil, Sai, Sarayu)  
**Sprint:** 3  
**Traceability:** `FR-GSM-001..004`, `FR-PRS-001..003`, `FR-DIF-001..003`, `FR-HSC-001..005`

## Objective

Integrate and validate application state management (`GameStateFlow`), pause/resume/restart (`GameplaySession`), difficulty selection (`Easy`, `Medium`, `Hard`), and high-score persistence (`HighScoreManager`) into a single cohesive workflow without breaking module boundaries.

## Integrated Architecture

- **`snakearcade::model::GameConfig` & `snakearcade::model::GameTypes`** (`src/model/GameConfig.hpp`, `src/model/GameTypes.hpp`)
  - Single canonical `enum class Difficulty { Easy, Medium, Hard }` and `enum class GameState { Start, Playing, Paused, GameOver }`.
  - Canonical movement intervals via `movementInterval(Difficulty)`:
    - `Easy`: `0.20F` s (`EasyMovementInterval`)
    - `Medium`: `0.15F` s (`MediumMovementInterval`)
    - `Hard`: `0.08F` s (`HardMovementInterval`)
- **`snakearcade::controller::GameStateFlow`** (`src/controller/GameStateFlow.hpp`, `src/controller/GameStateFlow.cpp`)
  - Pure C++ state machine (no SFML headers) owning `GameState` (`Start`, `Playing`, `Paused`, `GameOver`) and selected `Difficulty` (`Medium` by default).
  - Supports `selectDifficulty(Difficulty)` exclusively in `State::Start`, `startSession(...)`, `pause()`, `resume()`, `finishSession()`, and `returnToStart()` (`GameOver -> Start`).
- **`snakearcade::model::GameplaySession`** (`src/model/GameplaySession.hpp`, `src/model/GameplaySession.cpp`)
  - Owns active snake movement, food/score updates, collision detection, difficulty interval, and pause state (`pause()`, `resume()`, `isPaused()`, `restart(Difficulty)`).
  - Ignores direction inputs and frame advances while paused or finished, preserving the movement timer remainder across pause/resume.
  - Prevents rapid two-key neck reversals within a single tick (`stepPosition(snake_.head(), direction) == segments[1]`).
- **`snakearcade::persistence::HighScoreManager`** (`src/persistence/HighScoreManager.hpp`, `src/persistence/HighScoreManager.cpp`)
  - Loads stored high scores on startup; missing, empty, whitespace-only, signed (`+`/`-`), decimal, overflowing (`> INT_MAX`), or otherwise malformed files load as `0`.
  - Updates and flushes to disk only when a finished score strictly exceeds the current high score.
- **`snakearcade::controller::GameCoordinator`** (`src/controller/GameCoordinator.hpp`, `src/controller/GameCoordinator.cpp`)
  - SFML-free integration coordinator connecting `GameStateFlow`, `GameplaySession`, and `HighScoreManager`.
  - Enforces valid state transitions, pauses/resumes/restarts both state flow and gameplay session together, checks for session completion in `update()`, finishes into `GameOver`, and saves a new high score only when beaten.
- **`GameController`** (`src/controller/GameController.hpp`, `src/controller/GameController.cpp`)
  - Thin SFML presentation/input layer delegating all state, gameplay, and high-score logic to `GameCoordinator`.
  - Loads `assets/fonts/tuffy.ttf` (with `../assets/fonts/tuffy.ttf` and font-missing fallback) once during `run()`, and supports `1`/`2`/`3`, `Enter`, Arrow keys/`WASD`, `P`, `R`, and `Esc`/`M`.

## Integration Defects Found and Fixed

1. **Disconnected state flow and controller:** Replaced ad-hoc controller flags with `GameCoordinator` backed by `GameStateFlow`.
2. **Duplicate `Difficulty` enum definitions:** Consolidated into one canonical definition in `src/model/GameConfig.hpp` aliased in `src/model/GameTypes.hpp`.
3. **Startup high-score overwrite risk:** Stopped calling `save()` unconditionally during initialization so missing or malformed files are preserved until a strictly higher score is achieved.
4. **Per-frame font reload & asset path failure:** Loaded `assets/fonts/tuffy.ttf` once in `GameController::run()`, added `../assets/fonts/tuffy.ttf` plus shape-based fallback, and added a CMake `POST_BUILD` step to copy `assets/` alongside `snakearcade.exe`.
5. **Missing `GameOver -> Start` binding:** Wired `Esc` and `M` on `GameOver` to `GameCoordinator::returnToStart()`.
6. **Same-tick two-key neck reversal:** Added next-head-vs-neck guard inside `GameplaySession::requestDirection()`.
7. **Strict high-score parsing:** Replaced permissive `std::ifstream >>` parsing with strict non-negative ASCII integer validation and overflow protection.
8. **Repository hygiene:** Excluded `.vscode/` and runtime `highscore.txt` via `.gitignore`.

## Verification

Automated tests cover unit contracts (`GameStateFlowTests`, `GameplaySessionTests`, `HighScoreManagerTests`, `SnakeTests`, `MovementTimerTests`, `FoodScoreManagerTests`, `CollisionManagerTests`, `TestRunnerTests`) and end-to-end `GameCoordinator` integration (`Day15IntegrationTests`).

Run from the repository root in an MSYS2 UCRT64 terminal:

```bash
cmake -S . -B build -G "MinGW Makefiles" -DBUILD_TESTING=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Confirmed `100% tests passed, 0 tests failed out of 74` alongside clean `snakearcade.exe` and `snakearcade_tests.exe` builds.

