# Day 19: Defect Fix and Regression Results

## Defect reviewed

Issue #39 tracked a requirements gap against SRS FR-FGS-005: the score changed in game state and appeared in the window title, but the gameplay surface did not show a score HUD. This is a user-visible requirement, so it was treated as high priority.

## Correction

- Added an in-window HUD showing current score and persisted high score during gameplay, pause, and game-over states.
- Added a 40-pixel HUD strip and shifted the board drawing area down so all 40 by 30 cells remain visible.
- Kept the window title values as a secondary status aid.

## Execution record

- Test date: 2026-11-11
- Base revision: `fc6bece` (`main`, after PR #42)
- Environment: Windows 10, MinGW-w64 GCC 15.2.0, CMake 4.4.4, SFML fetched through CMake FetchContent
- Configure: `cmake -S . -B build-day19 -G "MinGW Makefiles" -DBUILD_TESTING=ON`
- Build: `cmake --build build-day19 --parallel`
- Test command: `ctest --test-dir build-day19 --output-on-failure`
- Actual result: **75/75 automated tests passed** (CTest exit code 0)
- Manual GUI smoke check: launched the game, started Easy mode, and observed `SCORE: 0` and `HIGH SCORE: 0` in the gameplay HUD. The HUD remained visible on the game-over screen. The snake reached a wall while the UI was being inspected, so a non-zero score update was not manually observed in this run.

## Regression / limitations

- The full existing unit and integration suite passed after the rendering changes.
- The HUD reads `coordinator_.score()` and `coordinator_.highScore()` each render; automated model tests cover score and high-score state changes. No automated pixel/render test exists, and this manual smoke check verified the visible zero-score HUD rather than visually capturing a food collection and non-zero value.
- Other previously documented limitations remain in the Day 17 and Day 18 result files. No further defect was discovered in this regression pass.
