cmake_minimum_required(VERSION 3.20)

project(SnakeArcade VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

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
)

target_compile_features(snakearcade PRIVATE cxx_std_17)
target_link_libraries(snakearcade PRIVATE sfml-graphics sfml-window sfml-system)
