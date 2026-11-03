# Day 9: Collision Detection

Day 9 establishes wall collision and self-collision detection in SnakeArcade, determining terminal states and game-over transitions.

## Collision Types (`CollisionManager`)

`CollisionManager` classifies collisions into three states defined in `GameTypes.hpp`:
- `Collision::None`: The proposed move is safe and valid.
- `Collision::Wall`: The proposed next head position lies outside the grid boundaries (`0 <= x < 40`, `0 <= y < 30`).
- `Collision::Self`: The snake's head intersects an existing body segment.

## Collision Rules

1. **Wall Collisions**: Evaluated using `isWithinBoard(nextHead)`.
2. **Self-Collisions**: Evaluated against the snake's segments. Short snakes (length <= 3) cannot self-collide. Moving into the currently occupied tail cell is legal during normal movement because the tail vacates that cell, but causes a collision if the tail is retained (such as during food consumption / growth).
3. **Precedence**: Boundary wall collisions take precedence over self-collision checks.

## Game-Over Behavior

- When a collision is detected during update ticks:
  1. The snake's movement halts immediately (model freezes).
  2. The controller enters the `isGameOver` state.
  3. The window title updates with the final score and a restart prompt.
  4. The snake renders in a distinct game-over state.
  5. The player can press `R` to restart a new game session.

