#include "model/Rules.hpp"
#include <cmath>

namespace Quoridor {

    bool Rules::isValidMove(const Board& board, int player, int fromRow, int fromCol, int toRow, int toCol) {
        // 1. Check bounds
        if (!Board::isInBounds(toRow, toCol)) {
            return false;
        }

        // 2. Check orthogonal movement (step size 1)
        int dRow = std::abs(toRow - fromRow);
        int dCol = std::abs(toCol - fromCol);
        
        if ((dRow + dCol) != 1) {
            // Diagonal or >1 step not allowed in basic validation (ignoring jumps for now)
            return false; 
        }

        // 3. Check if target cell is occupied
        // Iterate over all players to see if anyone is at (toCol, toRow)
        // Note: Board uses (x, y) -> (col, row)
        for (int p = 0; p < NUM_PLAYERS; ++p) {
            Position pos = board.getPawnPosition(p);
            if (pos.x == toCol && pos.y == toRow) {
                return false; // Target occupied
            }
        }
        
        // 4. Check for walls blocking the path
        if (isPathBlockedByWall(board, fromRow, fromCol, toRow, toCol)) {
            return false;
        }

        return true;
    }

    bool Rules::checkVictory(const Board& board, int player) {
        Position pos = board.getPawnPosition(player);
        
        // Player 0 starts at y=0, wins at y=8 (bottom)
        if (player == 0 && pos.y == BOARD_SIZE - 1) {
            return true;
        }
        
        // Player 1 starts at y=8, wins at y=0 (top)
        if (player == 1 && pos.y == 0) {
            return true;
        }
        
        return false;
    }

    bool Rules::isPathBlockedByWall(const Board& board, int fromRow, int fromCol, int toRow, int toCol) {
        const auto& walls = board.getWalls();
        
        // Determine direction
        bool movingVertical = (std::abs(toRow - fromRow) == 1);
        
        for (const auto& wall : walls) {
            // Wall coordinates (x, y) refer to the top-left corner of the wall intersection
            // Vertical wall at (x, y) separates col x and x+1, spanning rows y and y+1
            // Horizontal wall at (x, y) separates row y and y+1, spanning cols x and x+1
            
            if (movingVertical) {
                // Moving between rows. Blocked by Horizontal walls.
                // Movement between row 'r' and 'r+1' is blocked by Horizontal wall at row 'r'.
                // The wall spans columns wall.x and wall.x + 1.
                // We are moving in column 'fromCol'.
                // So if wall.y == min(fromRow, toRow) AND (wall.x == fromCol OR wall.x == fromCol - 1)
                
                if (wall.orientation == Orientation::Horizontal) {
                    int splitRow = std::min(fromRow, toRow);
                    if (wall.pos.y == splitRow) {
                        if (wall.pos.x == fromCol || wall.pos.x == fromCol - 1) {
                            return true;
                        }
                    }
                }
            } else {
                // Moving Horizontal (between columns). Blocked by Vertical walls.
                // Movement between col 'c' and 'c+1' is blocked by Vertical wall at col 'c'.
                // The wall spans rows wall.y and wall.y + 1.
                // We are moving in row 'fromRow'.
                // So if wall.x == min(fromCol, toCol) AND (wall.pos.y == fromRow OR wall.pos.y == fromRow - 1)
                
                if (wall.orientation == Orientation::Vertical) {
                    int splitCol = std::min(fromCol, toCol);
                    if (wall.pos.x == splitCol) {
                        if (wall.pos.y == fromRow || wall.pos.y == fromRow - 1) {
                            return true;
                        }
                    }
                }
            }
        }
        
        return false;
    }

} // namespace Quoridor
