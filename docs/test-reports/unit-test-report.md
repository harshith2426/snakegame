# Unit Test Report

## Execution record

- Date: 2026-11-11
- Source revision: `a645a2f7e67304a2003b2fe2a6e691abf28f53c5` (`main`, after PR #43)
- Platform: Windows 10 x64
- Compiler: MinGW-w64 GCC 15.2.0
- Build system: CMake 4.4.4 with MinGW Makefiles
- Test framework: GoogleTest 1.15.2 (CMake FetchContent)
- Configure: `cmake -S . -B build-part3-coverage -G "MinGW Makefiles" -DBUILD_TESTING=ON "-DCMAKE_CXX_FLAGS=--coverage -O0 -g" "-DCMAKE_EXE_LINKER_FLAGS=--coverage"`
- Build: `cmake --build build-part3-coverage --parallel`
- Test command: `ctest --test-dir build-part3-coverage --output-on-failure`
- Result: **75/75 CTest cases passed; 0 failed** (CTest exit code 0)
- Raw output: [`logs/coverage-build.log`](logs/coverage-build.log) and [`logs/unit-test-run.log`](logs/unit-test-run.log)

The build was performed from the repository state at the recorded revision, with SFML and GoogleTest fetched by CMake. CTest discovers individual GoogleTest cases. The suite covers snake movement and growth, timer cadence, food/scoring, collision, gameplay session coordination, game-state transitions, difficulty, and high-score persistence/error handling.

## Failures and limitations

- No automated test failed in this run.
- This report records automated unit/integration tests, not completion of the full manual STP workflow.
- GoogleTest and SFML are fetched from their pinned upstream tags during configure; an initial build requires network access.
- The coverage build instruments test-target production logic. GUI rendering and `main` are excluded from the coverage denominator; see the separate coverage reports.
