# Required Installations — SnakeArcade

This guide prepares each teammate's Windows computer to build SnakeArcade with C++17, CMake, MinGW-w64, and SFML 2.6.2.

## Required tools

| Tool | Why the project needs it | Install / check |
|---|---|---|
| Git | Clone the repository and let CMake fetch SFML | Install [Git for Windows](https://git-scm.com/download/win). Check in a terminal with `git --version`. |
| MSYS2 UCRT64 with MinGW-w64 GCC and Make | Provides the 64-bit Windows C++ compiler and build tool | Install [MSYS2](https://www.msys2.org/), then follow the setup below. Check with `g++ --version` and `mingw32-make --version`. |
| CMake | Reads the project build instructions and generates the native build | Install [CMake](https://cmake.org/download/) or its MSYS2 UCRT64 package. Check with `cmake --version`. |

**SFML 2.6.2 is fetched and built by the repository's CMake configuration.** Do not install SFML separately or add local SFML include/library paths. The first configure needs an internet connection.

## Install the compiler toolchain

1. Install MSYS2 from [msys2.org](https://www.msys2.org/).
2. Open the **MSYS2 UCRT64** terminal from the Start menu.
3. Update MSYS2 packages. If it asks you to close the terminal, close it, open UCRT64 again, and repeat the update command until it completes:

   ```bash
   pacman -Syu
   ```

4. In the UCRT64 terminal, install Git, the compiler, CMake, and Make:

   ```bash
   pacman -S --needed git mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make
   ```

5. Verify the tools in the **MSYS2 UCRT64** terminal:

   ```bash
   g++ --version
   cmake --version
   mingw32-make --version
   git --version
   ```

Use the UCRT64 terminal for configure/build commands so CMake finds the matching compiler and Make. CMake GUI and Visual Studio are optional; the shared team instructions use the terminal.

## Get, configure, and build the project

In the UCRT64 terminal, move to the parent folder where you keep projects, then run:

```bash
git clone https://github.com/harshith2426/snakegame.git
cd snakegame
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

The first configure fetches and builds the pinned SFML release and may take a few minutes. Generated files stay in `build/`; do not commit that folder. If CMake cached another compiler or generator, remove the `build/` folder and configure again from the UCRT64 terminal.

## Team consistency

- Build for 64-bit Windows with the MSYS2 UCRT64 MinGW-w64 toolchain.
- Let the repository's CMake configuration select the SFML version; do not use a separately installed SFML copy.
- Keep generated build output out of Git.
- Coordinate changes to the compiler toolchain, SFML version, and dependency method because they affect the whole team's setup.

## References

- [MSYS2 installation and environments](https://www.msys2.org/)
- [MSYS2 environments: UCRT64](https://www.msys2.org/docs/environments/)
- [MSYS2 CMake instructions](https://www.msys2.org/docs/cmake/)
- [SFML 2.6 CMake guide](https://www.sfml-dev.org/tutorials/2.6/getting-started/cmake/)
