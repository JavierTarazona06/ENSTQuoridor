# ENSTQuoridor 🎮

A strategic board game implementation in C++ with SFML, featuring AI opponents and a clean MVC architecture.

![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)
![License](/)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)

## 📖 About

**Quoridor** is a two-player strategy game played on a 9×9 board. Players take turns either moving their pawn one square or placing a wall between two squares. The objective is to reach the opposite row while respecting the key rule: always leave at least one valid path for each player to their goal.

This project is a modern C++ implementation featuring:
- **Human vs Human** and **Human vs AI** game modes
- **Configurable AI** with three difficulty levels (Easy/Normal/Hard)
- **Clean MVC architecture** for maintainability and testability
- **Cross-platform support** (Windows, Linux, macOS)
- **Advanced algorithms**: Minimax with α-β pruning, A*/BFS pathfinding

**Team**: Tarazona Javier, Liang Tianyi  
**Course**: IN204 - Object-Oriented Programming, ENSTA Paris  
**Defense Date**: January 15, 2026

---

## ✨ Features

### Gameplay
- ✅ **9×9 game board** conforming to official Quoridor rules
- ✅ **Pawn movement** with orthogonal moves and jump mechanics
- ✅ **Wall placement** with collision detection and path validation
- ✅ **Victory detection** when a player reaches the opposite row
- ✅ **Turn-based gameplay** with alternating players

### Artificial Intelligence
- 🤖 **Minimax/Negamax algorithm** with α-β pruning
- 🎯 **Smart heuristics**: distance estimation (A*), wall impact evaluation
- 🎲 **Three difficulty levels**:
  - **Easy**: Shallow search depth, high randomization
  - **Normal**: Balanced strategy
  - **Hard**: Deep search, minimal randomization
- ⚡ **Optimized performance**: <3 seconds per move

### Technical
- 🏗️ **MVC Architecture**: Model (game logic), View (SFML rendering), Controller (input/game loop)
- 🧪 **Unit tests** for rules and pathfinding (Catch2 framework)
- 📦 **Cross-platform packaging**: ZIP (Windows), DMG (macOS), AppImage (Linux)
- 🎨 **SFML-based 2D rendering** with responsive UI

---

## 🚀 Quick Start

### Prerequisites

- **C++20** compatible compiler (GCC 10+, Clang 11+, MSVC 2019+)
- **CMake** 3.20 or higher
- **SFML** 2.6 (installed via vcpkg, Conan, or system package manager)

### Build Instructions

See the complete platform-specific guide here: `docs/installations/game.md`.

---

## 📂 Project Structure

```
ENSTQuoridor/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── src/
│   ├── app/                    # Main entry point, SceneManager
│   ├── controller/             # InputHandler, Game loop, GameState
│   ├── model/                  # Board, Rules, State, Pathfinder, AI
│   └── view/                   # Renderer2D, UIWidgets, ResourceCache
├── include/                    # Public headers
├── assets/                     # Textures, fonts, sounds
├── tests/                      # Unit tests (Catch2)
├── docs/
│   ├── exigences/              # Requirements specification (French)
│   ├── architecture/           # Architecture documentation (French)
│   └── planning/               # Development plan and Trello tasks
├── scripts/                    # Build and packaging scripts
└── packaging/                  # CPack configuration, icons
```

---

## 🧩 Game Rules

### Core Rules
- **Objective:** Reach the opposite row before your opponent.
- **Turn options:**
  - **Move pawn:** One cell orthogonally (up, down, left, right).
  - **Place wall:** Between two cells; consumes one of your walls.
- **Jumping:**
  - If the opponent is directly adjacent, you may jump over them if the square behind them is free.
  - If the straight jump is blocked or off-board, a **diagonal jump** to one of the two adjacent diagonals is allowed.
- **Wall constraint:** A wall must not completely block any player's path to their goal — at least one path must remain.
- **Inventory:** Each player starts with **10 walls**.
- **Victory:** First to reach their goal row wins (Player 0 → bottom row, Player 1 → top row).

### Game Modes
- **Human vs Human**: Local multiplayer on the same machine
- **Human vs AI**: Challenge the computer at three difficulty levels


### Controls
- **Mouse:**
  - Click your pawn to select; click a destination cell to move.
  - Click the selected pawn again to deselect.
- **Keyboard:**
  - `W` — Toggle wall placement mode.
  - `O` — Rotate wall (horizontal ↔ vertical).
  - `1` / `2` / `3` / `4` — AI difficulty (Easy / Normal / Hard / Hell) in PVE demo.
  - `R` — Reset game.
  - `ESC` — Pause menu.

See also the detailed PVE demo guide: [docs/PVE_DEMO_GUIDE.md](docs/PVE_DEMO_GUIDE.md)

---

## 📦 Packaging for Distribution

Generate platform-specific executable packages:

```bash
# After building in Release mode
cmake --build build --config Release
cmake --install build --prefix out

# Generate packages with CPack
cd build
cpack -G ZIP          # Windows
cpack -G DragNDrop    # macOS
cpack -G TGZ          # Linux
```

**Deliverables**:
- `Quoridor-1.0-Windows.zip` (portable, includes SFML DLLs)
- `Quoridor-1.0-macOS.dmg` (drag-and-drop installer)
- `Quoridor-1.0-Linux-x86_64.AppImage` (single executable, no dependencies)

---

## 🛠️ Development Roadmap

### ✅ Iteration 1 (Nov 18 - Dec 1, 2025)
- Infrastructure setup (CMake, SFML, Git)
- Basic MVC architecture
- 9×9 board rendering
- Simple pawn movement
- Unit tests framework

### 🚧 Iteration 2 (Dec 2 - Dec 15, 2025)
- Complete movement rules (jumps)
- Wall placement with validation
- BFS pathfinding for path verification
- Victory detection
- Human vs Human mode

### 📅 Iteration 3 (Dec 16 - Jan 5, 2026)
- AI implementation (Minimax + α-β pruning)
- A* heuristics
- Three difficulty levels
- Menu system and UI improvements

### 📅 Iteration 4 (Jan 6 - Jan 14, 2026)
- Cross-platform packaging
- UI polish (textures, fonts, sounds)
- Documentation and user guide
- Defense presentation preparation

---

## 🤝 Contributing

This is an academic project with a fixed team. However, suggestions and feedback are welcome via GitHub Issues.

**Team Members**:
- **Javier Tarazona** - [@JavierTarazona06](https://github.com/JavierTarazona06)
- **Tianyi Liang**

---

## 📚 Documentation

- [Requirements Specification](docs/exigences/exigences_quoridor.pdf) (French)
- [Architecture Documentation](docs/architecture/architecture_quoridor.pdf) (French)
- [Development Plan](docs/planning/planification_quoridor.pdf) (French)
- [Iteration 1 Tasks](docs/planning/iteration1_trello_tasks.md)

---

## 📄 License

This project is developed as part of the IN204 course at ENSTA Paris. All rights reserved by the authors.

---

## 🙏 Acknowledgments

- **ENSTA Paris** for the IN204 Object-Oriented Programming course
- **SFML Team** for the excellent multimedia library
- Quoridor board game by Mirko Marchesi

---

**Project Status**: 🚧 In Development (Iteration 2)  
**Last Updated**: December 2, 2025
