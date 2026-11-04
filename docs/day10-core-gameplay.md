# Day 10: Integrate and validate core gameplay

`GameplaySession` now coordinates the model features without depending on an SFML window: timed movement, direction changes, food consumption, growth, scoring, wall/self collision, restart, and the full-board win condition. The SFML controller forwards input and frame time to the session and renders its state. This keeps the integrated rules testable without opening a window.

## Gameplay rules

- A movement tick advances one cell at the configured 0.15 second interval.
- Immediate direction reversals are rejected.
- Food is spawned only in a free board cell. Eating it grows the snake, awards the configured points, and spawns the next food.
- A wall or body collision ends the session without moving the snake into the colliding cell.
- After game over, movement and direction input stop. Press **R** to reset the snake, timer, score, and food.
- Filling the board ends the session with a win rather than leaving the game in a no-food state.

## Verification

The model-level `GameplaySessionTests` cover initial state, movement timing, food/growth/scoring integration, wall and self collisions, reversal rejection, frozen game-over state, restart, and a full-board win. Run the complete test suite with the CMake/CTest commands in the root README.

The full game build and automated suite were verified for this change on Windows with MinGW. Manual visual play testing is still useful on each teammate's machine for keyboard/window behavior.
