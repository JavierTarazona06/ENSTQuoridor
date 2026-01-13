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

See the packaging documentation for instructions on creating distributable packages.
