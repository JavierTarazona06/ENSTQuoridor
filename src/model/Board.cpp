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
        
        placedWalls_.clear();
        std::cout << "Board initialized. Player 0 at (" << pawnPositions_[0].x << ", " << pawnPositions_[0].y << ")." << std::endl;
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

    void Board::placeWall(const Wall& wall) {
        if (!isWallPlacementValid(wall.pos)) {
            throw std::invalid_argument("Wall position is outside the valid 8x8 wall grid.");
        }

        // Overlap/Path validation is deferred to the Rules class
        
        placedWalls_.push_back(wall);
    }

    Position Board::getPawnPosition(int playerIndex) const {
        if (playerIndex < 0 || playerIndex >= NUM_PLAYERS) {
            throw std::out_of_range("Invalid player index.");
        }
        return pawnPositions_[playerIndex];
    }

    const std::vector<Wall>& Board::getWalls() const {
        return placedWalls_;
    }

} // namespace Quoridor
