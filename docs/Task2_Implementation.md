# Task 2: Model Implementation (Board, State, Rules)

This document details the implementation of the core Model classes for the Quoridor game, completing Tasks 2.1, 2.2, and 2.3.

## 1. Board (Task 2.1)
**Goal**: Represent the physical game components.

*   **Grid**: 9x9 array of `Cell` structures.
*   **Pawns**: Tracks positions of 2 players (`pawnPositions_`).
*   **Walls**: Tracks placed walls (`placedWalls_`).
*   **Key Methods**:
    *   `setPawnPosition(player, row, col)`
    *   `placeWall(wall)`
    *   `isInBounds(row, col)`
*   **Files**: `include/model/Board.hpp`, `src/model/Board.cpp`.
*   **Tests**: `tests/test_board.cpp` (Initialization, Movement, Wall placement).

## 2. State (Task 2.2)
**Goal**: Manage dynamic game state.

*   **Composition**: Holds the `Board` object.
*   **Players**: Tracks `currentPlayer_` (0 or 1).
*   **Status**: Tracks `GameStatus` (Playing, Player1Won, Player2Won).
*   **Key Methods**:
    *   `switchPlayer()`
    *   `getGameStatus()`
*   **Files**: `include/model/State.hpp`, `src/model/State.cpp`.
*   **Tests**: `tests/test_state.cpp` (Player switching, Board integration).

## 3. Rules (Task 2.3)
**Goal**: Validate logic and win conditions.

*   **Validation** (`isValidMove`):
    *   **Bounds**: Target must be within [0, 8].
    *   **Orthogonal**: Movement allowed only up/down/left/right by 1 cell.
    *   **Collision**: Target cell must be empty.
    *   **Walls**: Cannot move through placed walls.
*   **Victory** (`checkVictory`):
    *   Player 0 wins at row 8.
    *   Player 1 wins at row 0.
*   **Files**: `include/model/Rules.hpp`, `src/model/Rules.cpp`.
*   **Tests**: `tests/test_rules.cpp` (Valid/Invalid moves, Wall blocking, Victory).

---

## How to Build and Run Tests

The project uses **CMake**. Ensure you have SFML installed.

### 1. Build
Run from the project root:

```bash
mkdir -p build
cd build
cmake ..
make
```

### 2. Run Tests

You can run each test suite individually to verify specific components:

**Test Board:**
```bash
./test_board
```

**Test State:**
```bash
./test_state
```

**Test Rules:**
```bash
./test_rules
```

### 3. Expected Output
All tests should pass with `[PASS]` messages and a final success confirmation.

```text
✅ All Board tests passed!
✅ All State tests passed!
✅ All Rules tests passed!
