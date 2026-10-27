# Day 2 — Architecture and Module Boundaries

**Project:** SnakeArcade

**Reference baseline:** original Synopsis v1.0 and combined SRS/SAD/Test Plan v1.0, plus the current sprint board assignments

**Status:** Day 2 design proposal for the team to review before implementation

## Day 2 outcome

This document turns the approved requirements and SAD component list into module boundaries, owner boundaries, and internal C++ interfaces. It does not change the SRS feature scope. The architecture follows the MVC structure in the SAD: controller/input, game model, SFML view, and isolated local persistence.

## Team ownership to use

The sprint board's current task assignments and the Sai/Rohil swap are the working ownership source. The original PDFs remain unchanged historical baselines; their older names are not the active assignment list.

| Teammate | Work to own | Board-aligned modules |
|---|---|---|
| Sai Harshith R | Snake movement, directional control, core game loop, input, rendering/UI integration | Game Controller loop, Input Handler, Snake movement, Renderer / Game View |
| Rohil Thimmanahalli Girish | Food generation, collision detection, growth, and score calculation | Collision Manager, Food & Score Manager |
| Sarayu Mayakonda | Start/pause/game-over management, difficulty levels, and high-score persistence | Game State flow, difficulty configuration, Persistence Manager |

Integration and system validation are shared work. The assignment table keeps the board's individual tasks intact and describes who coordinates each module boundary.

## Architecture and responsibilities

Keep the SAD component names so that the requirements and test plan still trace to the agreed design. Day 2 makes one boundary explicit: the loop/controller code owned by Sai delegates state-transition policy to the game-state flow owned by Sarayu. This avoids two competing copies of the current game state.

| Component | Responsibility and boundary | Owner |
|---|---|---|
| `InputHandler` | Reads SFML input events and converts them to logical direction/action requests. Does not mutate game state. | Sai |
| `GameController` | Owns the application loop and elapsed-time scheduling. Sends actions to the state flow and model; asks the renderer to draw a read-only snapshot. | Sai |
| `GameStateFlow` | Owns Start, Playing, Paused, and GameOver transitions; handles start/restart/pause/resume actions and selected difficulty. May be implemented as a small part of the controller layer. | Sarayu |
| `GameLogic` | Owns the model state and deterministic per-tick rules. Coordinates movement, collision, food consumption, growth, and score updates without depending on SFML. | Sai (movement/session loop) with Rohil (food/collision/scoring) |
| `Snake` | Stores ordered grid segments (head first), current direction, and legal direction changes; advances one cell per game tick. | Sai |
| `CollisionManager` | Checks wall and self-collision for a proposed next snake position and returns a collision result. | Rohil |
| `FoodScoreManager` | Chooses an unoccupied food cell, detects consumption, and applies the score/growth result. | Rohil |
| `Renderer` / `GameView` | Draws Start, Playing, Paused, and GameOver screens from a snapshot. Owns SFML drawing only, not game rules. | Sai |
| `PersistenceManager` | Reads, validates, and writes the local high-score value. | Sarayu |

### State ownership

The `GameStateFlow` owns the application mode; `GameLogic` owns the snake, food, and score model. The controller asks the state flow whether a game tick may run. A terminal collision reported by the model causes the state flow to enter GameOver. This separates the SAD's controller/state operations from the Sai/Rohil gameplay responsibilities on the board.

## Shared model types

Keep types used by the model independent of SFML:

```cpp
#include <optional>
#include <vector>

struct GridPosition {
    int x;
    int y;
};

enum class Direction { Up, Down, Left, Right };
enum class GameState { Start, Playing, Paused, GameOver };
enum class Difficulty { Easy, Medium, Hard };
enum class Collision { None, Wall, Self };

struct ModelSnapshot {
    std::vector<GridPosition> snake; // head first
    std::optional<GridPosition> food; // empty only when the board is full
    int score;
};

struct GameSnapshot { // composed by GameController for the view
    GameState state;
    Difficulty difficulty;
    ModelSnapshot model;
    int highScore;
};
```

Grid width/height, cell size, and each difficulty's tick interval come from one configuration definition. The model uses grid coordinates and elapsed seconds; only the input/view adapters translate to or from SFML types.

## Internal interface contracts

These are proposed C++ interfaces for coordinating the modules. They preserve the SAD operations and make their inputs and failure behavior explicit.

```cpp
class InputHandler {
public:
    void handleInput(const sf::Event& event);
    std::optional<Direction> getRequestedDirection();
    // Returns and clears the pending direction; no request means no change.
};

class GameStateFlow {
public:
    GameState state() const;
    Difficulty difficulty() const;
    void startSession(Difficulty difficulty); // selects difficulty and enters Playing
    void pause();                             // only Playing -> Paused
    void resume();                            // only Paused -> Playing
    void finishSession();                     // enters GameOver once
    void returnToStart();
};

class GameController {
public:
    void startSession(Difficulty difficulty); // also resets GameLogic
    void pause();
    void resume();
    void update(float elapsedSeconds);        // input, fixed-step model update, state check
    GameSnapshot snapshot() const;            // combines state-flow and model snapshots
};

class GameLogic {
public:
    void reset(Difficulty difficulty);
    bool applyDirection(Direction direction); // false if it reverses into the neck
    void update(float elapsedSeconds);         // advances zero or more due ticks
    ModelSnapshot getState() const;            // read-only gameplay model
};

class CollisionManager {
public:
    Collision checkWallCollision(GridPosition nextHead) const;
    Collision checkSelfCollision(const std::vector<GridPosition>& nextSnake,
                                 bool tailMoves) const;
};

class FoodScoreManager {
public:
    std::optional<GridPosition> spawnFood(
        const std::vector<GridPosition>& occupied);
    bool isConsumed(GridPosition head, GridPosition food) const;
    int applyScore(int currentScore); // returns updated score; caller grows snake
};

class PersistenceManager {
public:
    int loadHighScore();                // 0 if missing, unreadable, malformed, or out of range
    bool saveHighScore(int score);      // false on write failure
};

class Renderer {
public:
    void render(const GameSnapshot& snapshot); // view-only; never mutates model/state
};
```

`GameStateFlow` is the single source of truth for application state and selected difficulty. `GameLogic` is the single source of truth for the snake, food, and score. `GameController::update(dt)` asks the state flow whether ticks may run, forwards a pending direction, updates the model only while Playing, and composes a `GameSnapshot` for the view from both sources. On a terminal collision, it asks the state flow to enter GameOver and saves the score only if it beats the loaded high score.

## State transitions

| Current state | Input/result | Next state | Required action |
|---|---|---|---|
| Start | Start action | Playing | Controller resets snake/food/score using selected difficulty |
| Playing | Pause action | Paused | Freeze game ticks; retain the current model |
| Paused | Resume action | Playing | Resume ticks from the retained model |
| Playing | Wall or self collision | GameOver | Freeze model; compare and persist a new high score |
| GameOver | Restart action | Playing | Controller resets the session while keeping the all-time high score |
| Any | Invalid action for that state | unchanged | Ignore safely; do not mutate the model |

Difficulty selection is accepted before a new session starts. The selected tick interval is applied by the controller/model configuration, not by rendering frame rate.

## Key rules and error behavior

- Reject a 180-degree reversal before the next tick; no movement occurs because of an invalid direction.
- Compute the next snake body before collision checks. A move into the current tail cell is legal only when that tail cell vacates during the same tick.
- Food is placed only on an unoccupied cell. If there is no free cell, `spawnFood` returns `std::nullopt`; it must not retry forever.
- A collision ends the active session once; later updates while GameOver do not move the snake or repeatedly save the score.
- Missing/corrupt/out-of-range high-score data starts with 0 and produces a development warning; it must not crash the game.
- Persistence writes only to the configured local high-score file and only when a score exceeds the stored value.
- SFML window/input failures are handled in the outer application/controller layer; no SFML exception or type is part of a model interface.

## Dependency direction

```text
SFML events -> InputHandler -> GameController -> GameStateFlow
                                      |               |
                                      +-> GameLogic <-+
                                      |    |       |
                                      |    +-> CollisionManager
                                      |    +-> FoodScoreManager
                                      +-> Renderer / GameView (snapshot only)
GameStateFlow / GameController -> PersistenceManager -> High-Score File
```

`GameLogic`, `Snake`, `CollisionManager`, `FoodScoreManager`, `GameStateFlow`, and shared model types must not include SFML headers. The view never writes state back to the model.

## Suggested source layout

This is a proposed layout for the implementation work that follows Day 2; it does not create or assign implementation files yet.

```text
src/
  main.cpp
  controller/
    GameController.hpp/.cpp
    InputHandler.hpp/.cpp
    GameStateFlow.hpp/.cpp
  model/
    GameTypes.hpp
    GameLogic.hpp/.cpp
    Snake.hpp/.cpp
    CollisionManager.hpp/.cpp
    FoodScoreManager.hpp/.cpp
  view/
    Renderer.hpp/.cpp
  persistence/
    PersistenceManager.hpp/.cpp
tests/
  unit/
  integration/
```

## Requirement and test traceability

| SRS group / quality requirement | Architecture boundary | Evidence from the v1.0 Test Plan |
|---|---|---|
| FR-GSM-001–004: session start, game over, restart | GameController + GameStateFlow | session/system cases in the STP |
| FR-MOV-001–004: movement and direction | GameLogic + Snake + InputHandler | movement and reversal unit/system cases |
| FR-FGS-001–005: food, growth, score | FoodScoreManager + GameLogic | food/growth/scoring cases |
| FR-CGS-001–004: wall/self collision | CollisionManager + GameStateFlow | wall/self-collision cases |
| FR-PRS-001–003: pause/resume | GameStateFlow + GameController | pause/resume state-preservation case |
| FR-DIF-001–003: difficulty | GameStateFlow/config + GameLogic | distinct-speed case |
| FR-HSC-001–005 and SEC-001–003 | PersistenceManager | persistence and malformed-file cases |
| NFR-PERF-001: frame-rate independent movement | elapsed-time update in GameController/GameLogic | frame-rate comparison case |

## Day 2 review checklist

- Sai, Rohil, and Sarayu agree that the ownership rows match the current GitHub sprint board.
- Each module's public operations and failure behavior are agreed before implementation begins.
- The team confirms the `GameController` / `GameStateFlow` split and the single owner for the authoritative state value.
- Interface names and SRS/STP traceability are updated together if the team requests a change.

## Source documents

- [Original project synopsis v1.0](../SnakeArcade_Synopsis_TEAM1_SECTIONG.pdf)
- [Original combined SRS, SAD, and Software Test Plan v1.0](../SnakeArcade_Combined_SRS_SAD_TestPlan_SECTIONG_TEAM1.pdf)
