# Day 17: Core Gameplay and Collision Test Results

## Execution record

- Test date: 2026-11-11
- Revision under test: `f910aa7` on `main`, plus the Day 17 frame-rate test in this branch
- Environment: Windows 10, MinGW-w64 GCC 15.2.0, CMake 4.4.4, SFML fetched through CMake FetchContent
- Configure: `cmake -S . -B build-day17 -G "MinGW Makefiles" -DBUILD_TESTING=ON`
- Build: `cmake --build build-day17 --parallel`
- Test command: `ctest --test-dir build-day17 --output-on-failure`
- Actual result: **75/75 automated tests passed** (CTest exit code 0)
- Evidence type: automated unit and integration tests. This report does not claim that every case below was manually executed through the graphical interface.

## STP case results

The case IDs and requirement mappings follow Section 13.1 of the project SRS/SAD/Test Plan. Automated names below identify the existing regression coverage; the Day 17 case adds a direct elapsed-time cadence comparison.

| STP case | Requirement / expected behavior | Automated evidence | Result |
| --- | --- | --- | --- |
| TC-01 | FR-MOV-002: movement advances by one grid cell at each movement tick | `SnakeTests.MovesOneGridCellAndKeepsItsLength`; `MovementTimerTests.DoesNotMoveBeforeInterval`; `MovementTimerTests.PreservesRemainderAndCatchesUpElapsedTime` | Pass (automated) |
| TC-02 | FR-MOV-004: an immediate 180-degree reversal is rejected | `SnakeTests.RejectsImmediateReverseAndAllowsPerpendicularTurns`; `GameplaySessionTests.RejectsImmediateReverseAndDoesNotAdvanceAfterGameOver` | Pass (automated) |
| TC-03 | FR-FGS-002/003/004: food is consumed, snake grows, and score increases | `GameplaySessionTests.CoordinatesMovementFoodGrowthAndScoring`; related `FoodScoreManager` and `Snake` unit tests | Pass (automated) |
| TC-04 | FR-CGS-001: wall collision ends play without moving beyond the board | `GameplaySessionTests.WallCollisionEndsSessionWithoutMovingOutsideBoard`; `SnakeTests.DoesNotMoveOutsideTheBoard` | Pass (automated) |
| TC-05 | FR-CGS-002: self-collision ends play and does not award food | `GameplaySessionTests.SelfCollisionEndsSessionWithoutAwardingFood` | Pass (automated) |
| TC-10 | NFR-PERF-001: movement cadence is based on elapsed time rather than rendered frame count | `MovementTimerTests.SameElapsedTimeProducesSameTicksAtDifferentFrameRates` simulates eight seconds at 60 Hz and 144 Hz; each produces 53 ticks at a 0.15-second movement interval | Pass (automated simulation) |

## Limitations and follow-up

- TC-10 compares simulated elapsed-time inputs. Separate manual runs at two display/render frame rates were not performed.
- Graphical play, keyboard input, and rendered collision feedback were not exhaustively system-tested in this execution. The automated tests validate the underlying model/session behavior.
- The requirements review found that score is present in the window title but is not yet drawn in the gameplay area. This is tracked separately as issue #39; TC-03 here verifies score state changes, not the missing on-board HUD. Re-run the visual score check after that defect is fixed.
- Test report scope is limited to the STP cases assigned to Day 17. Day 18 covers pause, difficulty, persistence, and malformed high-score data; the final test-report deliverable is tracked in issue #20.
