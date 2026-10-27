# Required Installations — SnakeArcade

This guide prepares each teammate's Windows computer for the SnakeArcade C++17 project. Follow it before building the Day 3 project skeleton.

## Required tools

| Tool | Why the project needs it | Install / check |
|---|---|---|
| Git for Windows | Clone the repository and let CMake download the pinned SFML source | Install from [git-scm.com/download/win](https://git-scm.com/download/win), then run `git --version` in PowerShell. |
| C++17 compiler and Windows SDK | Compile the game for 64-bit Windows | Install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) or [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/). In the installer, select **Desktop development with C++** and include the MSVC x64/x86 build tools, a Windows 10 or 11 SDK, and C++ CMake tools for Windows. |
| CMake | Configure the project and build files | Install from [cmake.org/download](https://cmake.org/download/) or use the CMake component included with Visual Studio. Ensure `cmake` is available in PowerShell; check with `cmake --version`. |

This project will use **SFML 2.6.2 through CMake FetchContent**, pinned by the repository's CMake configuration. Teammates should not install a separate SFML copy or choose a different SFML version. The first CMake configure needs an internet connection to fetch SFML. This keeps the dependency version consistent and avoids machine-specific SFML paths.

## Optional tools

- **Visual Studio Code** or another editor/IDE, if you prefer it. The compiler and CMake still need to be installed separately as described above.
- **CMake GUI** is optional. The command-line instructions below are the shared workflow, so nobody needs CMake GUI to build the project.
- GitHub CLI, a separate debugger, and vcpkg are not required for the documented workflow.

## Install and verify

1. Install Git for Windows.
2. Install Visual Studio Community or Build Tools with the C++ workload and components listed above.
3. Install CMake if it is not already available. During installation, enable the option to add CMake to the system PATH, or use the CMake bundled with Visual Studio.
4. Close and reopen PowerShell so it picks up any PATH changes.
5. Verify the command-line tools:

   ```powershell
   git --version
   cmake --version
   ```

   To verify MSVC, open **Developer PowerShell for Visual Studio** from the Start menu and run:

   ```powershell
   cl
   ```

   The compiler should print its version and usage. An error saying `cl` is not recognized in ordinary PowerShell does not necessarily mean MSVC is missing; check it in Developer PowerShell.

## Get the project

In PowerShell, go to the folder where you keep your projects, then run:

```powershell
git clone https://github.com/harshith2426/snakegame.git
cd snakegame
```

After the Day 3 CMake files are added, configure and build from the repository root with:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

The first configure may take longer while CMake downloads and builds SFML. Build output goes in the ignored `build/` directory; do not commit it. If CMake cannot find a compiler, rerun the commands from **Developer PowerShell for Visual Studio** or confirm the C++ workload installation.

## Team consistency

- Use the repository's CMake configuration to select SFML; do not add local SFML include/library paths.
- Use a 64-bit MSVC build on Windows so the team uses the same compiler family and target architecture.
- Keep generated build files and downloaded dependency sources out of Git. Commit project source and configuration only.
- Ask the team before changing the compiler, SFML version, or dependency method, since those choices affect every teammate.

## References

- [Visual Studio C++ workload](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation)
- [CMake with Visual Studio](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio)
- [SFML 2.6 CMake project guide](https://www.sfml-dev.org/tutorials/2.6/getting-started/cmake/)
