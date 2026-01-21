# PVE Visual Demo Guide

This document explains how to build and run the PVE (Player vs AI) visual demo for testing the AI engine.

## Overview

The `test_pve_visual` is a standalone executable that allows you to play Quoridor against the AI using the existing graphical interface. This demo is designed to verify the AI engine functionality without modifying any existing project code.


## Building the Demo

The `test_pve_visual` is built automatically along with all other targets when you run the bootstrap scripts.

### Method 1: One-Command Bootstrap (Recommended)

This is the simplest way to build the entire project including the PVE demo.

**Windows (PowerShell):**
```powershell
# From repo root
scripts\bootstrap.ps1 -Config Debug
```

**Linux/macOS (Bash):**
```bash
# From repo root
chmod +x scripts/bootstrap.sh
scripts/bootstrap.sh Debug
```

### Method 2: Build Only the PVE Demo

If the project is already configured, you can build just the PVE demo target:

**Windows:**
```powershell
cmake --build build/x64-windows --target test_pve_visual --config Debug
```

**Linux:**
```bash
cmake --build build/x64-linux --target test_pve_visual
```

**macOS (Apple Silicon):**
```bash
cmake --build build/arm64-osx --target test_pve_visual
```

**macOS (Intel):**
```bash
cmake --build build/x64-osx --target test_pve_visual
```

### Method 3: Manual Configuration and Build

For advanced users who want full control:

**Windows (Visual Studio):**
```powershell
cd C:\path\to\ENSTQuoridor
cmake --preset x64-windows
cmake --build build/x64-windows --target test_pve_visual --config Debug
```

**Linux:**
```bash
cd /path/to/ENSTQuoridor
cmake --preset x64-linux
cmake --build build/x64-linux --target test_pve_visual
```

**macOS:**
```bash
cd /path/to/ENSTQuoridor
cmake --preset arm64-osx    # Apple Silicon
# or: cmake --preset x64-osx  # Intel
cmake --build build/arm64-osx --target test_pve_visual
```

## Running the Demo

### Output Locations

**Windows:**
```powershell
.\build\x64-windows\tests\Debug\test_pve_visual.exe
# Or for Release:
.\build\x64-windows\tests\Release\test_pve_visual.exe
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

## Game Controls

### Movement Mode (Default)

| Action | Description |
|--------|-------------|
| Click on your pawn | Select the pawn |
| Click on destination | Move the pawn to that cell |
| Click on selected pawn again | Deselect the pawn |

### Wall Placement Mode

| Key | Action |
|-----|--------|
| `W` | Toggle wall placement mode |
| `O` | Rotate wall (horizontal ↔ vertical) |
| Mouse move | Preview wall position |
| Left click | Place wall at preview position |

### AI Difficulty Selection

| Key | Difficulty | Description |
|-----|------------|-------------|
| `1` | Easy | Minimax depth 1, noise ±40 |
| `2` | Normal | Minimax depth 1, noise ±8 |
| `3` | Hard | Minimax depth 2, noise ±3 |
| `4` | Hell | Minimax depth 4, no noise |

#### Difficulty System Details

The AI difficulty is controlled by two factors:

1. **Search Depth**: How many moves ahead the AI looks
2. **Noise Injection**: Random value added to evaluation scores (±noise range)

| Difficulty | Depth | Noise | Suboptimal Rate | Behavior |
|------------|-------|-------|-----------------|----------|
| Easy | 1 | ±40 | ~50% | Sees immediate moves but often picks randomly |
| Normal | 1 | ±8 | ~15% | Sees immediate moves, occasionally makes mistakes |
| Hard | 2 | ±3 | ~5% | Can plan ahead, rarely makes mistakes |
| Hell | 4 | 0 | 0% | Deep calculation with perfect evaluation |

**Why Noise?**
- Without noise, even depth 1 AI would always play perfectly within its horizon
- Noise makes the AI occasionally pick suboptimal moves, simulating human-like mistakes
- Higher noise = more "mistakes" = easier to beat

**Suboptimal Rate Calculation:**
- When two moves differ by Δ points, the probability of choosing the worse move follows a triangular distribution
- For typical move differences (~10 points): Easy ~50%, Normal ~15%, Hard ~5%

**Strategy Tips:**
- **Easy**: Good for learning the game mechanics - AI makes many random choices
- **Normal**: Beatable by most players - focus on advancing while occasionally blocking
- **Hard**: Requires careful planning - control the center and use walls strategically  
- **Hell**: Expert level - only winnable with optimal play and some luck

### Other Controls

| Key | Action |
|-----|--------|
| `R` | Reset game |
| Close window | Exit game |

## Game Rules

- **Player 1 (Human)**: Red pawn, starts at top, goal is bottom row
- **Player 2 (AI)**: Blue pawn, starts at bottom, goal is top row
- Each player has 10 walls to place
- Walls cannot completely block a player's path to their goal
- First player to reach their goal row wins

## Technical Details

### Architecture

The demo reuses the following existing components:
- `Board` - Game board state management
- `State` - Game state (current player, game status)
- `Rules` - Move and wall validation
- `AI` - AI decision engine
- `Render2D` - SFML rendering
- `GameView` - Game visualization

### AI Implementation

The AI uses the Minimax algorithm with Alpha-Beta pruning and noise injection for difficulty balancing:

| Difficulty | Search Depth | Noise Level | Typical Response Time |
|------------|--------------|-------------|----------------------|
| Easy | 1 | ±40 | ~150μs |
| Normal | 1 | ±8 | ~150μs |
| Hard | 2 | ±3 | ~800μs |
| Hell | 4 | 0 | ~8-10ms |

#### Evaluation Function

The AI evaluates board positions using 5 weighted factors:

| Factor | Weight | Description |
|--------|--------|-------------|
| Distance Difference | ×10 | Difference in shortest path to goal |
| Endgame Bonus | ×15 | Extra value when close to winning |
| Wall Advantage | ×2 | Having more walls than opponent |
| Mobility | ×3 | Number of available moves |
| Wall Effectiveness | ×5 | Walls that block opponent vs waste walls |

### File Structure

```
tests/
├── test_pve_visual.cpp    # PVE demo source code
└── CMakeLists.txt         # Build configuration (includes test_pve_visual target)
```

## Troubleshooting

### General Issues

- **"Could not find toolchain file"**: Ensure `VCPKG_ROOT` is set or let the bootstrap scripts handle it
- **If build fails**: Delete the build folder and run bootstrap again
  - Windows: `Remove-Item -Recurse -Force .\build\x64-windows; scripts\bootstrap.ps1 -Config Debug`
  - Linux/macOS: `rm -rf build; scripts/bootstrap.sh Debug`
- **Verify SFML integration**: `cmake --preset x64-windows` (or `x64-linux`, `arm64-osx`) should show `-- Found SFML ...`

### SFML not found

Make sure vcpkg is properly configured and dependencies are installed:

**macOS:**
```bash
cmake --preset arm64-osx   # This should install SFML via vcpkg automatically
```

**Linux:**
```bash
# Option 1: Use vcpkg (recommended)
cmake --preset x64-linux

# Option 2: Install SFML system-wide
sudo apt-get install libsfml-dev    # Ubuntu/Debian
sudo dnf install SFML-devel         # Fedora
sudo pacman -S sfml                  # Arch Linux
```

**Windows:**
```powershell
# vcpkg should install SFML automatically with the preset
cmake --preset x64-windows
```

### Window doesn't appear

**Linux specific:** Make sure you have a display server running (X11 or Wayland).
```bash
echo $DISPLAY   # Should output something like ":0"
```

**Windows specific:** Make sure the SFML DLLs are in your PATH or in the same directory as the executable.

### Linux: "cannot open shared object file"

```bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
sudo ldconfig
```

### Windows: Missing DLL errors

1. Copy SFML DLLs from `build/vcpkg_installed/x64-windows/bin/` to the executable directory
2. Or add the vcpkg bin directory to your PATH

### Font loading errors

The demo requires fonts from the `assets/fonts/` directory. Make sure:
1. The `assets` folder exists in the project root
2. Font files are present in `assets/fonts/arial/`
3. You're running the executable from the project root directory

### AI takes too long

Try lowering the difficulty level (press `1` or `2`). Hell mode (depth 5) can take longer in complex board states.

## Verification Checklist

Use this demo to verify:

- [ ] AI responds to player moves correctly
- [ ] AI makes valid moves (pawns and walls)
- [ ] Different difficulty levels show different behaviors
- [ ] Victory detection works for both players
- [ ] Wall placement validation works correctly
- [ ] Game reset functions properly

## See Also

- [Game Installation Guide](installation/game.md) - Main game build and run instructions
- [User Guide](USER_GUIDE.md) - Main game user guide
- [Architecture](arquitecture/architecture_quoridor.pdf) - Project architecture
