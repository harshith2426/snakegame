# Requirements review — Issue #16

**Review date:** 2026-11-11  
**Reviewed revision:** `1afe559` (main after PRs #35, #37, and #38)  
**Baseline:** SnakeArcade Synopsis v1.0 and Combined SRS/SAD/Software Test Plan v1.0, including STP Sections 3, 5, 8, 13, Appendix C.

## Build and launch

The integrated Windows/MinGW build configured and compiled successfully with CMake. The automated CTest suite passed 74/74 cases. A manual launch displayed the difficulty start screen; Easy selection and Enter started gameplay; P displayed the pause screen; P resumed the same session. Manual checks here are limited to those actions; the detailed feature cases and persistence fixtures are recorded under issues #17 and #18.

## Requirements review

| Requirement group | Review result | Evidence / follow-up |
| --- | --- | --- |
| FR-GSM-001..004 | Implemented | Start screen, session initialization, collision/game-over, direct R restart are present in `GameController`/`GameCoordinator`; integration tests cover the flow. |
| FR-MOV-001..004; NFR-PERF-001..002 | Implemented | Arrow/WASD input, elapsed-time ticks, reverse-turn protection; model tests exercise movement and timer behavior. Manual keyboard/frame-rate cases are tracked under #17. |
| FR-FGS-001..005 | One gap found | Food placement, growth, points and spawning have automated coverage. **FR-FGS-005 score visibility is incomplete:** score appears in the window title but is not drawn as a live HUD in the gameplay area. Tracked as high-priority defect #39 for #19. |
| FR-CGS-001..004 | Implemented | Wall/self collision and terminal-session behavior have unit/integration coverage. Final screen walkthrough is included in #17. |
| FR-PRS-001..003 | Implemented | Paused sessions ignore input/time and preserve the session; automated integration tests and manual pause/resume check. |
| FR-DIF-001..003 | Implemented | Easy/Medium/Hard are selectable and use distinct intervals (0.20/0.15/0.08 s); integration tests exercise each interval. |
| FR-HSC-001..005; SEC-001..003; NFR-REL-001..003 | Implemented in code; negative cases automated | Load/save, strict validation, malformed/missing data, and update-only-on-new-record behavior have unit/integration tests. System validation of the configured runtime path is tracked under #18. |
| NFR-USE-001..002 | Partial | Start requires at most two keypresses; current score exists in the title but fails the in-game HUD interpretation of FR-FGS-005/NFR-USE-002. Defect #39. |
| SAD modularity/testability | Meets design intent | `GameCoordinator`, `GameStateFlow`, `GameplaySession`, and `HighScoreManager` keep core state/gameplay/persistence separate from SFML rendering; pure logic is covered without opening a window. |

## Defect and release plan

- **#39 — High:** draw the live score in the gameplay window and verify that it updates after food consumption.
- **#17:** record execution results for STP gameplay/input/timing/food/collision cases.
- **#18:** record state/difficulty/persistence and malformed-file validation results.
- **#19:** fix #39, add a regression check, and rerun the suite.
- **#20:** finalize the traceability, unit, system-validation, line-coverage, and branch-coverage reports; document any unrun cases and known limits.

This review is a requirements/code review plus a limited manual walkthrough, not the final system-validation sign-off. The remaining release work is explicit on the shared board; final readiness is deferred until #39 is resolved and the Part 3 evidence is complete.
