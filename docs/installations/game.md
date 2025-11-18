# Running the Game

This guide explains how to build and run the ENSTQuoridor application.

## Prerequisites

- **Make**: Required to execute build commands
- **vcpkg**: Must be installed and configured (see [vcpkg and SFML.md](vcpkg%20and%20SFML.md))
- **VCPKG_ROOT**: Environment variable must be set to your vcpkg installation path

## Available Commands

### Build the Project

Compiles the application and generates the executable:

```bash
make build
```

This will:
- Configure CMake with the x64-windows preset
- Install dependencies via vcpkg (if needed)
- Compile the source code
- Generate the executable in `build/x64-windows/Debug/hello_sfml.exe`

### Run the Application

Builds and executes the application:

```bash
make run
```

This will automatically build the project (if not already built) and run the executable.

### Clean Build Files

Removes all build artifacts and generated files:

```bash
make clean
```

This deletes the entire `build/` directory, forcing a fresh build on the next `make build`.

## Quick Start

```bash
# First time setup
$env:VCPKG_ROOT = "C:\dev\vcpkg"

# Build and run
make run
```

## Troubleshooting

**If `make` is not found:**
- Install Make for Windows via Chocolatey: `choco install make`
- Or use Git Bash which includes Make
- Or install MinGW which includes Make

**If build fails:**
- Ensure `VCPKG_ROOT` is set correctly
- Run `make clean` then `make build`
- Check that SFML is installed: `cmake --preset x64-windows`
