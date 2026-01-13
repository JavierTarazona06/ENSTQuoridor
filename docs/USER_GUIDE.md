# Quoridor User Guide

This guide explains the keyboard and mouse controls for the current version of the Quoridor game.

## 🎮 Basic Controls

- **Mouse**: Used for selecting pawns, moving pawns, and placing walls.
- **Keyboard**: Used for mode switching and game control.

## 🕹️ Detailed Interactions

### 1. Move Pawn Mode (Default)

The game starts in this mode.

- **Select Pawn**:
  - Hover over your pawn.
  - **Left Click** to select it.
  
- **Move Pawn**:
  - Once a pawn is selected, hover over the desired destination tile.
  - **Left Click** on the tile.
  - If the move is valid, the pawn will move to that position, and the turn will end, switching to the next player.
  - If the move is invalid, the pawn will not move.

- **Deselect**:
  - **Left Click** on the currently selected pawn again to deselect it.

### 2. Place Wall Mode

In this mode, you can place obstacles on the board.

- **Enter Mode**:
  - Press the **`W`** key on your keyboard.
  - You are now in wall placement mode.

- **Wall Preview**:
  - Move your mouse across the board. A semi-transparent preview of the wall will follow your cursor.

- **Rotate Wall**:
  - Press the **`O`** key.
  - This toggles the wall orientation between **Horizontal** and **Vertical**.

- **Confirm Placement**:
  - Move the mouse to your desired position.
  - **Left Click** to confirm placement.
  - If the placement is valid (no overlap, path not completely blocked), the wall is placed, one wall is deducted from your inventory, the turn ends, and the game **automatically switches back to Move Pawn mode**.
  - If the placement is invalid, the wall will not be placed.

- **Exit Mode**:
  - Press the **`W`** key again to cancel wall placement and return to Move Pawn mode manually.

### 3. Game Control

- **Reset Game**:
  - When the game is over (a player has reached the opposite side).
  - Press the **`R`** key to reset the board and start a new game.
