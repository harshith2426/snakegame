# System Validation Report

## Validation record

- Date: 2026-11-11
- Source revision: `a645a2f7e67304a2003b2fe2a6e691abf28f53c5` (`main`, after PR #43)
- Baseline: Combined SRS/SAD/Software Test Plan v1.0, STP Sections 3, 5, 8, 13, and 13.1
- Environment: Windows 10 x64, MinGW-w64 GCC 15.2.0, CMake 4.4.4, SFML 2.6.2 fetched through CMake
- Build: clean CMake configure and build succeeded; the 75-case automated suite passed.
- Overall: **automated logic validation passed; manual graphical system validation is partial**. This is not a claim that every STP step has been completed manually.

## Requirement validation matrix

| SRS requirement group | STP cases | Automated evidence | Observed/manual evidence | Status / remaining work |
|---|---|---|---|---|
| FR-GSM-001..004 — start/session/game-over/restart | TC-04, TC-05, TC-06 | `Day15IntegrationTest.*`, `GameplaySessionTests.*`, `GameStateFlowTests.*` | Earlier limited walkthrough launched from the menu, started Easy, paused, and resumed (requirements review). | Logic validated; not a full manual state-by-state system run. |
| FR-MOV-001..004 — direction and movement | TC-01, TC-02, TC-10 | `SnakeTests.*`, `MovementTimerTests.*`, `GameplaySessionTests.*` | Day 7 movement UI was observed previously; this execution did not repeat every keyboard case. | Automated pass; exhaustive keyboard/system test remains unrecorded. |
| FR-FGS-001..005 — food, growth, score and score display | TC-03 | `GameplaySessionTests.CoordinatesMovementFoodGrowthAndScoring`; `FoodScoreManagerTest.*` | PR #43 fixes defect #39 and visually confirmed score/high-score labels during play and game over; the smoke run did not observe a non-zero score. | Automated score-state pass and HUD visibility observed; visual increment after food is not confirmed manually. |
| FR-CGS-001..004 — wall/self collision | TC-04, TC-05 | `GameplaySessionTests.*`, `SnakeTests.*` | No exhaustive rendered collision walkthrough recorded. | Automated pass; manual visual confirmation remains. |
| FR-PRS-001..003 — pause/resume/state preservation | TC-06 | `GameplaySessionTests.*`, `Day15IntegrationTest.*`, `GameStateFlowTests.*` | Limited earlier P pause/resume walkthrough recorded in requirements review. | Automated pass; limited manual evidence only. |
| FR-DIF-001..003 — difficulty and speed | TC-07 | `GameplaySessionTests.*`, `Day15IntegrationTest.DifficultySelectionControlsSpeedAndSupportsReturnToStart` | No timing measurements through the GUI were recorded. | Automated pass; manual speed measurement remains. |
| FR-HSC-001..005, SEC-001..003, NFR-REL-001..003 — high-score storage and errors | TC-08, TC-09 | `HighScoreManagerTest.*`, `Day15IntegrationTest.*` | Full GUI persistence/relaunch and malformed-file app-launch checks were not recorded. | Automated pass; configured-runtime system checks remain. |
| NFR-PERF-001 — frame-rate-independent cadence | TC-10 | `MovementTimerTests.SameElapsedTimeProducesSameTicksAtDifferentFrameRates` | Simulated 60/144 Hz inputs, not two physical/rendered frame-rate runs. | Simulation pass; real-render-rate test remains. |
| NFR-USE-001..002 — usability and visible status | TC-03 plus walkthrough | State/UI code builds; automated state tests pass. | Earlier start/pause walkthrough and PR #43 score-label smoke check. | Partial manual validation; formal first-time-user task walkthrough remains. |

## Defects, deviations, and exclusions

- Issue #39 (live score HUD absent) was fixed in PR #43 and merged into `main`; score and high-score fields are now drawn during play, pause, and game over.
- The Day 19 UI smoke test did not collect food to demonstrate a non-zero displayed score. Automated tests cover score changes in the underlying game state; a manual non-zero HUD update should be included in the team demo checklist.
- SFML internals, Windows hardware/OS correctness, and exhaustive user-experience testing are outside the automated test scope.
- The report does not imply team sign-off. The team should review the remaining manual checks and confirm submission artifacts.

## Evidence

- [Updated STP case matrix](test-cases.md)
- [Unit Test Report](unit-test-report.md)
- [Day 17 gameplay results](day17-gameplay-test-results.md)
- [Day 18 state and persistence results](day18-state-persistence-results.md)
- [Day 19 defect regression results](day19-defect-regression-results.md)
- [Issue #16 requirements review](requirements-review.md)
