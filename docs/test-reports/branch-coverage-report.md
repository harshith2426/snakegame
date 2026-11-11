# Branch Coverage Report

## Execution record and scope

- Date: 2026-11-11
- Source revision: `a645a2f7e67304a2003b2fe2a6e691abf28f53c5` (`main`, after PR #43)
- Tool: MinGW-w64 gcov 15.2.0 with branch counts enabled (`-b -c`)
- Test suite: 75 CTest cases, all passed.
- Scope and exclusions: same eight production `.cpp` translation units as the [Code Coverage Report](code-coverage-report.md). Branch outcomes are counted from gcov JSON for those files only; test code, headers, third-party code, `main`, and the SFML renderer are excluded.
- Raw output: [`logs/gcov-raw.log`](logs/gcov-raw.log) and [`logs/coverage-json/`](logs/coverage-json/).

## Results

Each gcov branch record is an instrumented control-flow outcome. An outcome is counted covered when its execution count is greater than zero. This is distinct from the percentage of branches that were entered at least once printed in gcov's text summary.

| Production source file | Covered outcomes | Total outcomes | Outcome coverage |
|---|---:|---:|---:|
| `src/controller/GameCoordinator.cpp` | 21 | 26 | 80.77% |
| `src/controller/GameStateFlow.cpp` | 14 | 14 | 100.00% |
| `src/model/CollisionManager.cpp` | 13 | 20 | 65.00% |
| `src/model/FoodScoreManager.cpp` | 20 | 30 | 66.67% |
| `src/model/GameplaySession.cpp` | 49 | 63 | 77.78% |
| `src/model/MovementTimer.cpp` | 10 | 12 | 83.33% |
| `src/model/Snake.cpp` | 23 | 37 | 62.16% |
| `src/persistence/HighScoreManager.cpp` | 46 | 62 | 74.19% |
| **Total** | **196** | **264** | **74.24%** |

## Reproduction and known gaps

Use the coverage configure/build/test steps in the [Code Coverage Report](code-coverage-report.md), clear old `*.gcda` files before execution, then run gcov with `-b -c --json-format` for each tested `.cpp` using its corresponding `.gcno` file as `-o`. The compressed JSON files retain counts for inspecting individual unexecuted outcomes.

The 68 uncovered branch outcomes include unobserved alternative/error paths; they are not classified as defects solely by their coverage status. Review the raw JSON and add targeted tests only where a path represents required behavior. No branch percentage is inferred from line coverage. GUI/rendering branches remain outside this headless unit-test measurement and need manual/system evidence where relevant.
