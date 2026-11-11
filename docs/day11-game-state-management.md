# Day 11 — Game-State Management

**Owner:** Sarayu Mayakonda
**Sprint:** 3
**Scope:** Define and test the application's state transitions. Pause/resume/restart input and screen rendering are left for their separately assigned implementation day.

## Implementation

- Added `GameState` (`Start`, `Playing`, `Paused`, `GameOver`) and `Difficulty` (`Easy`, `Medium`, `Hard`) to `model/GameTypes.hpp`.
- Added `GameStateFlow`, which is the single owner of the current application state and selected difficulty.
- State-flow code has no SFML dependency, so it can be tested independently.
- Transition methods return `true` when a transition occurs and `false` when the requested transition is invalid. Invalid requests leave state unchanged.

## Transition rules

| Current state | Operation | Resulting state | Valid |
|---|---|---|---|
| Start | `startSession(difficulty)` | Playing | Yes |
| Playing | `pause()` | Paused | Yes |
| Paused | `resume()` | Playing | Yes |
| Playing | `finishSession()` | GameOver | Yes |
| GameOver | `startSession(difficulty)` | Playing | Yes (restart) |
| GameOver | `returnToStart()` | Start | Yes |
| Any other combination | Any operation | Unchanged | No |

The initial state is `Start`; the default selected difficulty is `Medium`. A successful `startSession` stores the requested difficulty.

## Unit tests

`tests/controller/GameStateFlowTests.cpp` verifies initial values, session start, pause/resume, game over, restart, return to start, and rejection of invalid transitions. Each invalid-transition test checks that the state remains unchanged.

## Build and test

From the project root:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Scope boundary

This day establishes the application state-management component and integrates it with the game controller.

The controller supports:
- Starting the game using Enter.
- Pausing and resuming using P.
- Restarting after Game Over using R.
- Preventing gameplay updates while in Start, Paused, or GameOver states.
- Detecting completed gameplay sessions and transitioning to GameOver.

The selected difficulty is stored by GameStateFlow. Difficulty-specific movement intervals are not implemented in this task.
