cmake_minimum_required(VERSION 3.20)

project(SnakeArcade VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
include(CTest)

# The starter uses only SFML graphics/window/system. Keep the dependency build
# small and compile it with the same MinGW toolchain as the game.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Disable unused SFML audio module" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Disable unused SFML network module" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

add_executable(snakearcade
    src/main.cpp
    src/controller/GameController.cpp
    src/model/Snake.cpp
    src/model/MovementTimer.cpp
)

target_include_directories(snakearcade PRIVATE src)
target_compile_features(snakearcade PRIVATE cxx_std_17)
target_link_libraries(snakearcade PRIVATE sfml-graphics sfml-window sfml-system)

if(BUILD_TESTING)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE
    )
    # Keep GoogleTest's runtime consistent with the MinGW toolchain on Windows.
    set(gtest_force_shared_crt ON CACHE BOOL "Use the shared runtime on Windows" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(snakearcade_tests
        tests/TestRunnerTests.cpp
        tests/model/SnakeTests.cpp
        tests/model/MovementTimerTests.cpp
        src/model/Snake.cpp
        src/model/MovementTimer.cpp
    )
    target_include_directories(snakearcade_tests PRIVATE src)
    target_compile_features(snakearcade_tests PRIVATE cxx_std_17)
    target_link_libraries(snakearcade_tests PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(snakearcade_tests)
endif()
