#include "model/Board.hpp"

namespace Quoridor {

    // --- Constructor ---
    Board::Board() {
        // Initialize grid cells
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                grid_[row][col] = {col, row};
            }
        }

        // Player 0 (top row, center column)
        pawnPositions_[0] = {BOARD_SIZE / 2, 0}; 
        // Player 1 (bottom row, center column)
        pawnPositions_[1] = {BOARD_SIZE / 2, BOARD_SIZE - 1}; 
        
        // Initialize pawn colors
        pawnColors_[0] = {255, 0, 0};   // Red for player 0
        pawnColors_[1] = {0, 0, 255};   // Blue for player 1
        
        // Initialize wall counts
        wallsRemaining_.fill(MAX_WALLS_PER_PLAYER);

        placedWalls_.clear();
        std::cout << "Board initialized." << std::endl;
    }

    void Board::resetGame() {
        // Reset pawn positions
        pawnPositions_[0] = {BOARD_SIZE / 2, 0}; 
        pawnPositions_[1] = {BOARD_SIZE / 2, BOARD_SIZE - 1}; 

        // Reset wall counts
        wallsRemaining_.fill(MAX_WALLS_PER_PLAYER);

        // Clear placed walls
        placedWalls_.clear();
    }

    // --- Helper Validation Methods ---
    bool Board::isPositionValid(const Position& pos) {
        return pos.x >= 0 && pos.x < BOARD_SIZE &&
               pos.y >= 0 && pos.y < BOARD_SIZE;
    }

    bool Board::isInBounds(int row, int col) {
        return row >= 0 && row < BOARD_SIZE &&
               col >= 0 && col < BOARD_SIZE;
    }

    bool Board::isWallPlacementValid(const Position& pos) {
        // Wall grid is 8x8 (0 to 7)
        return pos.x >= 0 && pos.x < (BOARD_SIZE - 1) &&
               pos.y >= 0 && pos.y < (BOARD_SIZE - 1);
    }

    // --- Core Methods ---

    void Board::movePawn(int playerIndex, const Position& newPos) {
        if (playerIndex < 0 || playerIndex >= NUM_PLAYERS) {
            throw std::out_of_range("Invalid player index.");
        }
        if (!isPositionValid(newPos)) {
            throw std::invalid_argument("Pawn target position is out of bounds.");
        }
        
        // Rules validation is handled by the Rules class (in Iteration 2)
        pawnPositions_[playerIndex] = newPos;
    }

    void Board::setPawnPosition(int player, int row, int col) {
        // Map row/col to Position (x=col, y=row)
        Position newPos = {col, row};
        movePawn(player, newPos);
    }

    void Board::placeWall(const Wall& wall, int playerIndex) {
        if (playerIndex < 0 || playerIndex >= NUM_PLAYERS) {
            throw std::out_of_range("Invalid player index.");
        }

        if (wallsRemaining_[playerIndex] <= 0) {
            throw std::runtime_error("Player has no walls remaining.");
        }

        if (!isWallPlacementValid(wall.pos)) {
            throw std::invalid_argument("Wall position is outside the valid 8x8 wall grid.");
        }

        // Check for overlaps
        for (const auto& existingWall : placedWalls_) {
            // 1. Exact position check (collision or crossing)
            if (existingWall.pos == wall.pos) {
                throw std::invalid_argument("Wall overlaps or crosses an existing wall.");
            }

            // 2. Partial overlap check
            if (wall.orientation == Orientation::Horizontal) {
                // If horizontal, cannot overlap with other horizontal walls at x-1 or x+1 (same y)
                if (existingWall.orientation == Orientation::Horizontal &&
                    existingWall.pos.y == wall.pos.y &&
                    (existingWall.pos.x == wall.pos.x - 1 || existingWall.pos.x == wall.pos.x + 1)) {
                    throw std::invalid_argument("Wall overlaps with another horizontal wall.");
                }
            } else { // Vertical
                // If vertical, cannot overlap with other vertical walls at y-1 or y+1 (same x)
                if (existingWall.orientation == Orientation::Vertical &&
                    existingWall.pos.x == wall.pos.x &&
                    (existingWall.pos.y == wall.pos.y - 1 || existingWall.pos.y == wall.pos.y + 1)) {
                    throw std::invalid_argument("Wall overlaps with another vertical wall.");
                }
            }
        }

        placedWalls_.push_back(wall);
        wallsRemaining_[playerIndex]--;
    }

    int Board::getWallsRemaining(int playerIndex) const {
        if (playerIndex < 0 || playerIndex >= NUM_PLAYERS) {
            throw std::out_of_range("Invalid player index.");
        }
        return wallsRemaining_[playerIndex];
    }

    bool Board::isWallAt(const Position& pos, Orientation orientation) const {
        Wall targetWall = {pos, orientation};
        for (const auto& wall : placedWalls_) {
            if (wall == targetWall) {
                return true;
            }
        }
        return false;
    }

    Position Board::getPawnPosition(int playerIndex) const {
        if (playerIndex < 0 || playerIndex >= NUM_PLAYERS) {
            throw std::out_of_range("Invalid player index.");
        }
        return pawnPositions_[playerIndex];
    }

    Color Board::getPawnColor(int playerIndex) const {
        if (playerIndex < 0 || playerIndex >= NUM_PLAYERS) {
            throw std::out_of_range("Invalid player index.");
        }
        return pawnColors_[playerIndex];
    }

    const std::vector<Wall>& Board::getWalls() const {
        return placedWalls_;
    }

} // namespace Quoridor
