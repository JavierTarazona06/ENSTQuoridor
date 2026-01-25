# Running the Game

This guide explains how to build and run the ENSTQuoridor application on Windows, Linux, and macOS.

## Prerequisites

### For Developers (Building from Source)

**Windows:**
- **Git**: https://git-scm.com/download/win
- **CMake 3.20+**: https://cmake.org/download/ (add to PATH during installation)
- **Visual Studio Build Tools 2019** or Visual Studio 2019/2022: https://visualstudio.microsoft.com/downloads/
  - During installation, select "Desktop development with C++"
- vcpkg: the bootstrap script will clone it locally if not already installed

**Linux:**
- Git, CMake 3.20+, GCC/Clang
- vcpkg: the bootstrap script will clone it locally if not already installed

**macOS:**
- Xcode Command Line Tools: `xcode-select --install`
- CMake 3.20+: `brew install cmake`
- vcpkg: the bootstrap script will clone it locally if not already installed

## Building from Source

### Complete Setup for New Users (Windows)

1. **Install prerequisites** (see above)

2. **Clone the repository:**
```powershell
git clone https://github.com/JavierTarazona06/ENSTQuoridor.git
cd ENSTQuoridor
```

3. **Run bootstrap script:**
```powershell
scripts\bootstrap.ps1 -Config Debug
```

4. **Run the application:**
```powershell
.\build\x64-windows\Debug\quoridor_game.exe
```

### Cross-Platform One-Command Bootstrap

These scripts set up vcpkg (if needed), select the right CMake preset, and build.

- Windows (PowerShell):
```powershell
# From repo root
scripts\bootstrap.ps1 -Config Debug

# Just program. From repo root
cmake --build build/x64-windows --config Debug
```

- Linux/macOS (Bash):
```bash
# From repo root
chmod +x scripts/bootstrap.sh
scripts/bootstrap.sh Debug

# Just program. From repo root
# For macOS (Apple Silicon):
cmake --build build/arm64-osx --config Debug
# For macOS (Intel) or Linux (adjust preset name as needed):
# cmake --build build/x64-osx --config Debug
# cmake --build build/x64-linux --config Debug
```

## Output

Executable location depends on your platform and configuration:

**Windows:**
```powershell
.\build\x64-windows\Debug\quoridor_game.exe
# Or for Release:
.\build\x64-windows\Release\quoridor_game.exe

# Run all tests:
ctest --test-dir build/x64-windows/tests -C Debug
# or
.\build\x64-windows\tests\Debug\test_all.exe

# View individual test:
.\build\x64-windows\tests\Debug\test_all.exe --list-tests
# Run individual test:
.\build\x64-windows\tests\Debug\test_all.exe [tags]
```

**Linux:**
```bash
./build/x64-linux/quoridor_game

# Run all tests:
ctest --test-dir build/x64-linux/tests -C Debug
# or
./build/x64-linux/tests/test_all

# View individual test:
./build/x64-linux/tests/test_all --list-tests
# Run individual test:
./build/x64-linux/tests/test_all [tags]
```

**macOS:**
```bash
./build/arm64-osx/quoridor_game
# Or try 
./build/x64-osx/quoridor_game

# Run all tests:
ctest --test-dir build/arm64-osx/tests -C Debug
  # or
  ./build/arm64-osx/tests/test_all
# Or for x64:
ctest --test-dir build/x64-osx/tests -C Debug
  # or
  ./build/x64-osx/tests/test_all

# View individual test:
./build/arm64-osx/tests/test_all --list-tests
# Run individual test:
./build/arm64-osx/tests/test_all [tags]
# Or for x64:
./build/x64-osx/tests/test_all --list-tests
./build/x64-osx/tests/test_all [tags]
```

### PVE Visual Demo

A visual demo (`test_pve_visual`) is also built automatically, allowing you to play against the AI:

**Windows:**
```powershell
.\build\x64-windows\tests\Debug\test_pve_visual.exe
```

**Linux:**
```bash
./build/x64-linux/tests/test_pve_visual
```

**macOS:**
```bash
./build/arm64-osx/tests/test_pve_visual    # Apple Silicon
./build/x64-osx/tests/test_pve_visual      # Intel
```

See [PVE Demo Guide](../PVE_DEMO_GUIDE.md) for detailed controls and usage.

## Troubleshooting

- **"Could not find toolchain file"**: Ensure `VCPKG_ROOT` is set or let the bootstrap scripts handle it
- **If build fails**: Delete the build folder and run bootstrap again
  - Windows: `Remove-Item -Recurse -Force .\build\x64-windows; scripts\bootstrap.ps1 -Config Debug`
  - Linux/macOS: `rm -rf build; scripts/bootstrap.sh Debug`
- **Verify SFML integration**: `cmake --preset x64-windows` (or `x64-linux`, `x64-osx`) should show `-- Found SFML ...`

## For End Users (No Development Tools Required)

If you want to distribute the application to users who don't have development tools installed, you'll need to create a portable package containing:
- The executable (`.exe` on Windows)
- SFML DLL files
- Assets folder

### Creating a Distributable ZIP Package (Windows)

First, build the Release version:

```powershell
# Configure (if not already done)
cmake --preset x64-windows

# Build Release
scripts\bootstrap.ps1 -Config Release
# Or just program
cmake --build build\x64-windows --config Release
```

Then generate the portable ZIP package:

```powershell
cd build\x64-windows
cpack -G ZIP -C Release
```

This creates `Quoridor-1.0-Windows.zip` containing:
- `quoridor_game.exe`
- SFML DLLs (`sfml-graphics-3.dll`, `sfml-window-3.dll`, `sfml-system-3.dll`, `sfml-audio-3.dll`)
- `assets/` folder with fonts and images

The ZIP can be distributed to users who can simply extract and double-click `quoridor_game.exe` to run the game.

### Creating a Distributable Package (Linux)

To generate end-user packages, run:

```bash
scripts/package-linux.sh
```

Recommended steps (tar.gz):
1. Open a terminal and extract the archive from the repository root:
   ```bash
   cd dist
   tar -xvzf Quoridor-x86_64.tar.gz
   ```
   This creates a new folder with the unpacked program.
2. Enter the extracted folder:
   ```bash
   cd quoridor
   ```
3. List files and locate the executable:
   ```bash
   ls -l
   ```
   Look for a file without an extension or with `.sh`/`.bin` (examples: `Quoridor`, `run.sh`, `start.sh`).
4. Grant execute permission if needed:
   ```bash
   chmod +x quoridor_game
   ```
   Replace `Quoridor` with the actual executable name.
5. Run the program:
   ```bash
   ./quoridor_game
   ```

An `Quoridor-x86_64.AppImage` is also produced in `dist/`. Make it executable and run it:
```bash
chmod +x Quoridor-x86_64.AppImage
./Quoridor-x86_64.AppImage
```
Some distributions require `libfuse2` to run AppImages.
