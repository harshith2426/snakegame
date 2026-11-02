# Day 6 — Snake Body Representation

The game model stores the snake independently of SFML so it can be tested without opening a window. `GridPosition` uses integer cell coordinates with `(0, 0)` at the top-left; x increases to the right and y increases downward. `Snake::segments()` returns the ordered body from head to tail, and `direction()` returns the current logical direction.

## Initial configuration

The starter window is 800 by 600 pixels. The model uses a 40-column by 30-row grid with 20 pixels per cell, so the board fits that window exactly. The snake starts at the center with three horizontal segments facing right:

| Segment | Grid cell |
|---|---|
| Head | `(20, 15)` |
| Body | `(19, 15)` |
| Tail | `(18, 15)` |

These values live in `GameConfig.hpp` so the renderer and later game logic can share one board definition. The selected length and board dimensions can be revised together if the team changes the board design.

## Future movement and growth

The head is always the first segment and the tail is always the last. Movement can insert a new head at the next grid cell and remove the tail; growth can insert the new head while retaining the tail. `occupies()` and `isWithinBoard()` provide model-only checks for food placement and collision handling. Direction changes and movement timing are implemented in the next movement task.
