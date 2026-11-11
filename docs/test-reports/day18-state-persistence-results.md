# Day 18: State, Difficulty, and Persistence Test Results

## Execution record

- Test date: 2026-11-11
- Revision under test: `1075d69` (`main`, after PR #41)
- Environment: Windows 10, MinGW-w64 GCC 15.2.0, CMake 4.4.4, SFML fetched through CMake FetchContent
- Configure: `cmake -S . -B build-day18-check -G "MinGW Makefiles" -DBUILD_TESTING=ON`
- Build: `cmake --build build-day18-check --parallel`
- Test command: `ctest --test-dir build-day18-check --output-on-failure`
- Actual result: **75/75 automated tests passed** (CTest exit code 0)
- Evidence type: automated unit and integration tests. No new manual GUI run is claimed in this report.

## STP case results

The case IDs and requirement mappings follow Section 13.1 of the SRS/SAD/Test Plan. TC-10 is recorded with the Day 17 timing report.

| STP case | Requirement / expected behavior | Automated evidence | Result |
| --- | --- | --- | --- |
| TC-06 | FR-PRS-001..003: pause stops play without losing the session; resume continues and restart resets the session | `GameplaySessionTests.PauseStopsMovementAndInputWhilePreservingTimerRemainder`; `Day15IntegrationTest.PausePreservesSessionAndRestartResetsSession`; `GameStateFlowTests.AllowsPauseAndResumeTransitions` | Pass (automated) |
| TC-07 | FR-DIF-001/002: Easy, Medium, and Hard use distinct movement intervals | `GameplaySessionTests.EasyDifficultyUsesSlowMovement`; `GameplaySessionTests.MediumDifficultyUsesNormalMovement`; `GameplaySessionTests.HardDifficultyUsesFastMovement`; `Day15IntegrationTest.DifficultySelectionControlsSpeedAndSupportsReturnToStart` | Pass (automated) |
| TC-08 | FR-HSC-003/004: a new high score is saved and reloaded; lower/equal scores do not replace it | `HighScoreManagerTest.SavesAndLoadsHighScore`; `HighScoreManagerTest.LowerScoreDoesNotReplaceHighScore`; `HighScoreManagerTest.EqualScoreDoesNotReplaceHighScore`; `Day15IntegrationTest.HighScoreUpdatesAndPersistsAcrossSessionsOnlyWhenBeaten` | Pass (automated) |
| TC-09 | FR-HSC-005 / SEC-001: missing, malformed, or invalid score data is handled safely; write failures do not corrupt the in-memory score | `HighScoreManagerTest.MissingFileLoadsZero`; `HighScoreManagerTest.MalformedFileLoadsZero`; `HighScoreManagerTest.ExtraDataIsRejected`; `HighScoreManagerTest.EmptyAndWhitespaceFilesLoadZero`; `HighScoreManagerTest.NegativeAndSignedValuesInFileLoadZero`; `HighScoreManagerTest.OverflowAndDecimalValuesInFileLoadZero`; `HighScoreManagerTest.UnwritablePathDoesNotOverwriteInMemoryHighScore`; `Day15IntegrationTest.MalformedScoreFileLoadsZeroAndRecoversWhenBeaten` | Pass (automated) |

Additional state-flow tests pass for starting a session, reaching game over, returning to start, rejecting invalid transitions, and preserving the selected difficulty after restart. See `GameStateFlowTests.*`, `GameplaySessionTests.*`, and `Day15IntegrationTest.*` in the test sources.

## Limitations and follow-up

- The unwritable-file test points the score writer at the temporary directory itself. It verifies the file-open/write failure path; it is not a separate test of Windows ACL permissions.
- These are automated tests of the state/session/persistence logic. A separate interactive keyboard/UI system-validation run was not performed for this report.
- Day 17's elapsed-time comparison and its limitations are documented in [Day 17 results](day17-gameplay-test-results.md).
- The gameplay score HUD defect remains tracked by issue #39 and is outside these state/persistence cases.
