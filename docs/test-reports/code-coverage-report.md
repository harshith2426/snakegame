# Code Coverage Report

## Execution record and scope

- Date: 2026-11-11
- Source revision: `a645a2f7e67304a2003b2fe2a6e691abf28f53c5` (`main`, after PR #43)
- Compiler/tool: MinGW-w64 GCC and gcov 15.2.0
- Instrumentation: `--coverage -O0 -g` for compile and link
- Test suite: 75 CTest cases, all passed; see [Unit Test Report](unit-test-report.md).
- Measurement scope: eight production `.cpp` files compiled into `snakearcade_tests` and exercised by the automated suite. GoogleTest, SFML, standard-library headers, test sources, `src/main.cpp`, and the SFML `GameController.cpp` renderer are excluded. GUI files are excluded because the headless test target does not execute them.
- Raw evidence: [`logs/gcov-raw.log`](logs/gcov-raw.log) and the per-translation-unit compressed JSON snapshots in [`logs/coverage-json/`](logs/coverage-json/).

## Results

| Production source file | Executed lines | Line coverage | Executed functions | Function coverage |
|---|---:|---:|---:|---:|
| `src/controller/GameCoordinator.cpp` | 79 / 84 | 94.05% | 21 / 22 | 95.45% |
| `src/controller/GameStateFlow.cpp` | 37 / 37 | 100.00% | 9 / 9 | 100.00% |
| `src/model/CollisionManager.cpp` | 22 / 28 | 78.57% | 5 / 5 | 100.00% |
| `src/model/FoodScoreManager.cpp` | 26 / 28 | 92.86% | 5 / 6 | 83.33% |
| `src/model/GameplaySession.cpp` | 95 / 98 | 96.94% | 18 / 18 | 100.00% |
| `src/model/MovementTimer.cpp` | 17 / 17 | 100.00% | 3 / 3 | 100.00% |
| `src/model/Snake.cpp` | 52 / 54 | 96.30% | 12 / 12 | 100.00% |
| `src/persistence/HighScoreManager.cpp` | 58 / 68 | 85.29% | 6 / 6 | 100.00% |
| **Total** | **386 / 414** | **93.24%** | **79 / 81** | **97.53%** |

## Reproduction

From the repository root using the documented MSYS2 UCRT64/MinGW environment:

```bash
cmake -S . -B build-part3-coverage -G "MinGW Makefiles" -DBUILD_TESTING=ON "-DCMAKE_CXX_FLAGS=--coverage -O0 -g" "-DCMAKE_EXE_LINKER_FLAGS=--coverage"
cmake --build build-part3-coverage --parallel
ctest --test-dir build-part3-coverage --output-on-failure
```

After clearing prior `*.gcda` files in the build tree, run gcov once per listed tested translation unit, passing that translation unit's `build-part3-coverage/CMakeFiles/snakearcade_tests.dir/<source-path>.gcno` file to `-o`. Example:

```bash
gcov -b -c --json-format -o build-part3-coverage/CMakeFiles/snakearcade_tests.dir/src/model/Snake.cpp.gcno src/model/Snake.cpp
```

The JSON snapshots preserve the exact gcov data used for this table. Totals include only executable line entries and functions in the eight scoped production `.cpp` files; header instantiations and third-party files are not added to the denominator.

## Interpretation

Coverage describes which instrumented logic ran during this suite. It does not prove correctness or complete SRS system validation. The 28 unexecuted source lines and two unexecuted functions are visible gaps; the GUI renderer and application entry point are not measured here. Coverage is reported separately from branch coverage in [Branch Coverage Report](branch-coverage-report.md).
