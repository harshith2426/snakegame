# Day 8: Food Generation, Snake Growth, and Scoring

Day 8 introduces food generation, food consumption detection, snake growth, and player scoring in SnakeArcade.

## Food Placement (`FoodScoreManager`)

- Food spawns at integer grid coordinates `(x, y)` bounded by `BoardColumns` (40) and `BoardRows` (30).
- `spawnFood()` queries all unoccupied board cells and selects one using uniform distribution, ensuring food never spawns on any cell occupied by the snake's body.
- If the board is completely full, `spawnFood()` safely returns `std::nullopt` without entering an infinite loop.
- A seeded constructor allows deterministic placement during automated unit tests.

## Consumption and Growth

- `isConsumed(head, food)` detects when the snake's head coordinate reaches the current food coordinate.
- When food is consumed:
  1. The snake increases in length by retaining its tail (`Snake::grow()`).
  2. The player's score increases by `PointsPerFood` (10 points).
  3. A new food item is generated on an unoccupied cell.

## Controller and View Integration

- `GameController` updates the score in the window title when food is eaten.
- `GameController::render()` draws food as a distinct red cell (`sf::Color(230, 60, 60)`) on the grid.

