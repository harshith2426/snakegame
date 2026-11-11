# Updated test cases and SRS traceability

**Baseline:** SnakeArcade Combined SRS/SAD/Software Test Plan v1.0, STP Section 13.1
**Execution revision:** `a645a2f` (main after PR #43)
**Execution date:** 2026-11-11
**Environment:** Windows 10, MinGW-w64 GCC 15.2.0, CMake 4.4.4, GoogleTest 1.15.2 fetched by CMake

The STP's ten case IDs are retained. “Pass (automated)” means the corresponding logic is exercised by automated tests; it does not claim the full keyboard-driven scenario was manually performed in the graphical application.

| STP case | SRS requirement(s) | Steps / expected result (summary) | Automated evidence | Actual result / status | Manual/system evidence and limits |
|---|---|---|---|---|---|
| TC-01 | FR-MOV-002 | Advance several updates; snake moves one grid cell per movement tick. | `SnakeTests.MovesOneGridCellAndKeepsItsLength`; `MovementTimerTests.DoesNotMoveBeforeInterval`; `MovementTimerTests.PreservesRemainderAndCatchesUpElapsedTime` | Pass (automated) | Automated model/timer evidence only; no separate multi-frame GUI measurement. |
| TC-02 | FR-MOV-004 | Turn right, request immediate reverse; reverse is rejected. | `SnakeTests.RejectsImmediateReverseAndAllowsPerpendicularTurns`; `GameplaySessionTests.RejectsImmediateReverseAndDoesNotAdvanceAfterGameOver` | Pass (automated) | Keyboard input is implemented by the UI; not exhaustively manually tested. |
| TC-03 | FR-FGS-002/003/004 | Consume food; snake grows, score increases, and food respawns. | `GameplaySessionTests.CoordinatesMovementFoodGrowthAndScoring`; `FoodScoreManagerTest.*`; `SnakeTests.Grow*` | Pass (automated) | The Day 19 UI smoke run did not observe a non-zero score; see [Day 19 results](day19-defect-regression-results.md). |
| TC-04 | FR-CGS-001 | Move into a wall; session ends without moving beyond board. | `GameplaySessionTests.WallCollisionEndsSessionWithoutMovingOutsideBoard`; `SnakeTests.DoesNotMoveOutsideTheBoard` | Pass (automated) | Logic test; rendered terminal screen is not exhaustively system-tested. |
| TC-05 | FR-CGS-002 | Grow snake and steer into its body; session ends without awarding food. | `GameplaySessionTests.SelfCollisionEndsSessionWithoutAwardingFood` | Pass (automated) | Logic test; complete keyboard path was not manually exercised. |
| TC-06 | FR-PRS-001/002/003 | Pause mid-session, wait, resume; state is preserved. | `GameplaySessionTests.PauseStopsMovementAndInputWhilePreservingTimerRemainder`; `Day15IntegrationTest.PausePreservesSessionAndRestartResetsSession`; `GameStateFlowTests.AllowsPauseAndResumeTransitions` | Pass (automated) | A limited earlier UI walkthrough verified P pause/resume; see [requirements review](requirements-review.md). |
| TC-07 | FR-DIF-001/002 | Start sessions at Easy, Medium, and Hard; intervals are distinct. | `GameplaySessionTests.EasyDifficultyUsesSlowMovement`; `GameplaySessionTests.MediumDifficultyUsesNormalMovement`; `GameplaySessionTests.HardDifficultyUsesFastMovement`; `Day15IntegrationTest.DifficultySelectionControlsSpeedAndSupportsReturnToStart` | Pass (automated) | No separate timed UI measurement at each difficulty was recorded. |
| TC-08 | FR-HSC-003/004 | Finish above stored record; high-score file updates and reloads. | `HighScoreManagerTest.SavesAndLoadsHighScore`; `HighScoreManagerTest.HigherScoreUpdatesHighScore`; `Day15IntegrationTest.HighScoreUpdatesAndPersistsAcrossSessionsOnlyWhenBeaten` | Pass (automated) | Runtime persistence through a full GUI session was not separately confirmed in this run. |
| TC-09 | FR-HSC-005, SEC-001 | Supply malformed score data; app recovers safely with zero rather than crashing. | `HighScoreManagerTest.MalformedFileLoadsZero`; `HighScoreManagerTest.ExtraDataIsRejected`; `HighScoreManagerTest.EmptyAndWhitespaceFilesLoadZero`; `HighScoreManagerTest.NegativeAndSignedValuesInFileLoadZero`; `HighScoreManagerTest.OverflowAndDecimalValuesInFileLoadZero`; `Day15IntegrationTest.MalformedScoreFileLoadsZeroAndRecoversWhenBeaten` | Pass (automated) | Automated parser/persistence tests; no separate malformed-file GUI launch was recorded. |
| TC-10 | NFR-PERF-001 | Compare equal elapsed time at two rendering update rates; movement tick count should match. | `MovementTimerTests.SameElapsedTimeProducesSameTicksAtDifferentFrameRates` | Pass (automated simulation) | Simulates 8 seconds at 60 Hz and 144 Hz, yielding 53 ticks at 0.15 seconds; no physical display/frame-rate measurement. |

## Deviations and outstanding validation

- All ten STP cases have automated logic evidence in the current 75-test suite. The detailed execution records are in [Day 17](day17-gameplay-test-results.md), [Day 18](day18-state-persistence-results.md), and the [Unit Test Report](unit-test-report.md).
- This table does not mark any unexecuted manual UI path as passed. Keyboard-driven system validation and real display-rate timing remain incomplete as manual evidence.
- TC-03's score state change is tested automatically; a non-zero score was not observed in the Day 19 graphical smoke run. That is recorded as a limitation, not hidden as a successful manual check.
- The source test plan is a planning baseline. Any new or revised test procedure should retain its TC identifier and be reviewed by the team against the approved PDF.
