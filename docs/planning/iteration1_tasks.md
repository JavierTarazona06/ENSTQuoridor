# Iteration 1: Foundations and Minimal Prototype
**Duration**: November 18 – December 1, 2025 (2 weeks)  
**Team**: Javier Tarazona, Tianyi Liang

---

## Trello Board Columns
- **To Do**
- **In Progress**
- **Testing/Review**
- **Done**

---

## Sprint Goals
- Set up development environment (CMake, SFML, Git)
- Implement basic MVC architecture
- Display 9×9 board with SFML
- Implement simple pawn movement with validation
- Create unit tests for movement rules

---

## Tasks Breakdown

### Infrastructure Setup

#### **TASK 1.1: Create Git Repository Structure**
- **Assigned to**: Javier
- **Estimate**: 2 hours
- **Description**: 
  - Initialize Git repository
  - Create folder structure: `src/`, `include/`, `assets/`, `tests/`, `docs/`, `cmake/`
  - Set up `.gitignore` for C++ (build/, bin/, .vscode/, etc.)
  - Create `README.md` with basic project description
  - Push initial structure to GitHub
- **Acceptance Criteria**:
  - Repository accessible by both team members
  - All directories created and committed
  - `.gitignore` excludes build artifacts

---

#### **TASK 1.2: Install and Configure SFML**
- **Assigned to**: Tianyi
- **Estimate**: 4 hours
- **Description**:
  - Install SFML 2.6 using vcpkg or Conan on Windows
  - Verify SFML installation with a "Hello SFML" window test
  - Document installation steps in `docs/setup.md`
  - Test on both team members' machines
- **Acceptance Criteria**:
  - SFML library available and linkable
  - Simple window opens successfully
  - Installation documented

---

#### **TASK 1.3: Create CMakeLists.txt**
- **Assigned to**: Javier
- **Estimate**: 3 hours
- **Description**:
  - Write `CMakeLists.txt` (minimum version 3.20, C++20)
  - Configure `find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)`
  - Create executable target linking SFML components
  - Add install rules for future packaging
  - Test build on Windows
- **Acceptance Criteria**:
  - `cmake -S . -B build` runs without errors
  - `cmake --build build` compiles successfully
  - Both team members can build the project

---

### Model (Game Logic)

#### **TASK 2.1: Implement Board Class**
- **Assigned to**: Tianyi
- **Estimate**: 6 hours
- **Description**:
  - Create `src/model/Board.h` and `src/model/Board.cpp`
  - Represent 9×9 grid (array or vector of cells)
  - Store positions of 2 pawns (Player 1 at row 0, Player 2 at row 8)
  - Implement `getPawnPosition(int player)`, `setPawnPosition(int player, int row, int col)`
  - Implement `isInBounds(int row, int col)`
- **Acceptance Criteria**:
  - Board initializes with correct starting positions
  - Getter/setter methods work correctly
  - Compiles without errors

---

#### **TASK 2.2: Implement State Class**
- **Assigned to**: Javier
- **Estimate**: 4 hours
- **Description**:
  - Create `src/model/State.h` and `src/model/State.cpp`
  - Track current player (1 or 2)
  - Store game status (Playing, Player1Won, Player2Won)
  - Implement `getCurrentPlayer()`, `switchPlayer()`, `getGameStatus()`
  - Link with Board instance (composition)
- **Acceptance Criteria**:
  - State correctly tracks active player
  - Player switching works
  - Compiles and links with Board

---

#### **TASK 2.3: Implement Rules Class (Basic Movement)**
- **Assigned to**: Tianyi
- **Estimate**: 8 hours
- **Description**:
  - Create `src/model/Rules.h` and `src/model/Rules.cpp`
  - Implement `isValidMove(Board& board, int player, int fromRow, int fromCol, int toRow, int toCol)`
  - Validate orthogonal movement (up/down/left/right by 1 cell)
  - Check bounds and ensure target cell is empty
  - Implement `checkVictory(Board& board, int player)` (player reaches opposite row)
- **Acceptance Criteria**:
  - Valid moves return true, invalid return false
  - Victory detection works correctly
  - Unit tests pass (see TASK 5.1)

---

### View (SFML Rendering)

#### **TASK 3.1: Implement Renderer2D Class**
- **Assigned to**: Javier
- **Estimate**: 8 hours
- **Description**:
  - Create `src/view/Renderer2D.h` and `src/view/Renderer2D.cpp`
  - Initialize SFML window (800×600 pixels)
  - Render 9×9 grid using `sf::RectangleShape` (draw cell borders)
  - Draw 2 pawns as colored circles (`sf::CircleShape`) at their positions
  - Implement `render(Board& board)` method
  - Handle window clear/display cycle
- **Acceptance Criteria**:
  - Window opens and displays grid
  - Pawns visible at correct starting positions
  - Grid lines clear and properly aligned

---

#### **TASK 3.2: Create Asset Folder and Basic Resources**
- **Assigned to**: Javier
- **Estimate**: 2 hours
- **Description**:
  - Create `assets/fonts/` directory
  - Add a free font file (e.g., Arial.ttf or open-source alternative)
  - Test loading font in SFML (`sf::Font`)
  - Document asset sources in `assets/README.md`
- **Acceptance Criteria**:
  - Font loads without errors
  - Assets folder structure ready for future textures/sounds

---

### Controller (Input & Game Loop)

#### **TASK 4.1: Implement Game Class**
- **Assigned to**: Tianyi
- **Estimate**: 6 hours
- **Description**:
  - Create `src/controller/Game.h` and `src/controller/Game.cpp`
  - Implement main game loop: poll events → update → render
  - Handle window close event
  - Integrate Board, State, Rules, Renderer2D
  - Implement `run()` method
- **Acceptance Criteria**:
  - Game loop runs at stable framerate
  - Window responds to close events
  - Classes interact correctly (Board ↔ State ↔ Renderer)

---

#### **TASK 4.2: Implement InputHandler Class**
- **Assigned to**: Javier
- **Estimate**: 6 hours
- **Description**:
  - Create `src/controller/InputHandler.h` and `src/controller/InputHandler.cpp`
  - Detect mouse clicks on grid cells
  - Convert pixel coordinates to grid coordinates (row, col)
  - Implement two-click move logic: 
    1. First click selects source cell (if pawn present)
    2. Second click attempts move to target cell
  - Validate and apply move via Rules class
  - Update State and Board on valid move
- **Acceptance Criteria**:
  - Clicking on pawn selects it (visual feedback optional)
  - Clicking valid target moves pawn
  - Invalid moves are rejected (no visual change)

---

#### **TASK 4.3: Create main.cpp Entry Point**
- **Assigned to**: Tianyi
- **Estimate**: 2 hours
- **Description**:
  - Create `src/app/main.cpp`
  - Instantiate Game object
  - Call `game.run()`
  - Handle any exceptions gracefully
- **Acceptance Criteria**:
  - Application launches and runs game loop
  - Clean exit on window close
  - No memory leaks (basic check with valgrind or similar)

---

### Testing

#### **TASK 5.1: Set Up Test Framework**
- **Assigned to**: Javier
- **Estimate**: 3 hours
- **Description**:
  - Integrate Catch2 test framework (via vcpkg or header-only)
  - Create `tests/CMakeLists.txt`
  - Add test executable to main `CMakeLists.txt`
  - Write sample test to verify framework works
- **Acceptance Criteria**:
  - `ctest` command runs successfully
  - Sample test passes

---

#### **TASK 5.2: Write Unit Tests for Rules Class**
- **Assigned to**: Tianyi
- **Estimate**: 5 hours
- **Description**:
  - Create `tests/test_rules.cpp`
  - Test valid moves (up, down, left, right by 1 cell)
  - Test invalid moves (diagonal, out of bounds, move >1 cell, target occupied)
  - Test victory conditions (player 1 reaches row 8, player 2 reaches row 0)
  - Aim for at least 10 test cases
- **Acceptance Criteria**:
  - All tests pass
  - Code coverage for Rules class >80%

---

#### **TASK 5.3: Write Unit Tests for Board Class**
- **Assigned to**: Javier
- **Estimate**: 3 hours
- **Description**:
  - Create `tests/test_board.cpp`
  - Test pawn position getters/setters
  - Test `isInBounds()` for edge cases
  - Test initial board state
- **Acceptance Criteria**:
  - All tests pass
  - Board logic verified

---

### Integration & Demo

#### **TASK 6.1: Integration Testing**
- **Assigned to**: Both (pair programming)
- **Estimate**: 4 hours
- **Description**:
  - Play a full game manually (click moves for both players)
  - Verify alternating turns
  - Test victory detection (move a pawn to opposite row)
  - Check for crashes or visual bugs
  - Document any issues found
- **Acceptance Criteria**:
  - Complete game playable from start to victory
  - No crashes during normal play
  - Issues logged for future iterations

---

#### **TASK 6.2: Prepare Iteration 1 Demo**
- **Assigned to**: Both
- **Estimate**: 2 hours
- **Description**:
  - Record short video (1-2 min) showing:
    - Window opening with grid and pawns
    - Moving pawns by clicking
    - Victory detection
  - Update `README.md` with build instructions and demo GIF/video
  - Tag Git commit as `v0.1-iteration1`
- **Acceptance Criteria**:
  - Demo video/GIF available
  - README includes clear build/run instructions
  - Git tag created

---

## 📊 Effort Summary

| Team Member | Estimated Hours | Tasks Assigned |
|-------------|-----------------|----------------|
| **Javier**  | ~37 hours       | 1.1, 1.3, 2.2, 3.1, 3.2, 4.2, 5.1, 5.3, 6.1, 6.2 |
| **Tianyi**  | ~37 hours       | 1.2, 2.1, 2.3, 4.1, 4.3, 5.2, 6.1, 6.2 |
| **Total**   | ~74 hours       | ~37h per person over 2 weeks (~18-19h/week) |

---

## Definition of Done (Iteration 1)

- [ ] All tasks marked as "Done" in Trello
- [ ] Code compiles without errors on Windows
- [ ] SFML window displays 9×9 board and 2 pawns
- [ ] Players can move pawns by clicking (valid moves only)
- [ ] At least 15 unit tests pass
- [ ] Victory is detected and displayed
- [ ] README contains build/run instructions
- [ ] Demo video/GIF available
- [ ] Git repository up-to-date with tag `v0.1-iteration1`

---

## Next Steps (Iteration 2 Preview)

- Implement wall placement
- Add jump mechanics (jumping over opponent)
- Implement BFS pathfinding to validate wall placements
- Improve UI (HUD, error messages)

---

## 📌 Notes

- **Daily Standups** (optional but recommended): 10-min sync every 2-3 days via chat/call
- **Code Review**: Each PR reviewed by the other team member before merging
- **Branch Strategy**: Create feature branches (`feature/board`, `feature/renderer`, etc.), merge to `main` when tested
- **Communication**: Use GitHub Issues or Trello comments for task-specific discussions

---

**Good luck, Javier and Tianyi! Let's build a solid foundation!**
