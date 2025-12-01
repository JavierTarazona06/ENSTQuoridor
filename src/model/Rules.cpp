#include "model/Rules.hpp"
#include <cmath>
#include <queue>
#include <set>

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

    bool Rules::isValidWallPlacement(const Board& board, const Wall& wall, int playerIndex) {
        // 1. Check walls remaining
        if (board.getWallsRemaining(playerIndex) <= 0) {
            return false;
        }

        // 2. Check bounds (0-7 for wall coordinates)
        if (wall.pos.x < 0 || wall.pos.x >= (BOARD_SIZE - 1) ||
            wall.pos.y < 0 || wall.pos.y >= (BOARD_SIZE - 1)) {
            return false;
        }

        // 3. Check overlaps with existing walls
        const auto& walls = board.getWalls();
        for (const auto& existingWall : walls) {
            // Exact position overlap (collision or crossing)
            if (existingWall.pos == wall.pos) {
                return false;
            }

            // Partial overlap
            if (wall.orientation == Orientation::Horizontal) {
                if (existingWall.orientation == Orientation::Horizontal &&
                    existingWall.pos.y == wall.pos.y &&
                    (existingWall.pos.x == wall.pos.x - 1 || existingWall.pos.x == wall.pos.x + 1)) {
                    return false;
                }
            } else { // Vertical
                if (existingWall.orientation == Orientation::Vertical &&
                    existingWall.pos.x == wall.pos.x &&
                    (existingWall.pos.y == wall.pos.y - 1 || existingWall.pos.y == wall.pos.y + 1)) {
                    return false;
                }
            }
        }

        // 4. Check if wall blocks all paths for any player
        if (!pathExists(board, 0, &wall)) return false;
        if (!pathExists(board, 1, &wall)) return false;

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

    // Helper function to check if a specific wall blocks the move
    static bool isBlockedBy(const Wall& wall, int fromRow, int fromCol, int toRow, int toCol) {
        bool movingVertical = (std::abs(toRow - fromRow) == 1);
        
        if (movingVertical) {
            // Moving between rows (up/down)
            // Blocked by Horizontal wall at min(fromRow, toRow)
            // Wall spans cols x and x+1.
            // We are at col fromCol.
            if (wall.orientation == Orientation::Horizontal) {
                int splitRow = std::min(fromRow, toRow);
                if (wall.pos.y == splitRow) {
                    if (wall.pos.x == fromCol || wall.pos.x == fromCol - 1) {
                        return true;
                    }
                }
            }
        } else {
            // Moving Horizontal (left/right)
            // Blocked by Vertical wall at min(fromCol, toCol)
            // Wall spans rows y and y+1.
            // We are at row fromRow.
            if (wall.orientation == Orientation::Vertical) {
                int splitCol = std::min(fromCol, toCol);
                if (wall.pos.x == splitCol) {
                    if (wall.pos.y == fromRow || wall.pos.y == fromRow - 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool Rules::isPathBlockedByWall(const Board& board, int fromRow, int fromCol, int toRow, int toCol, const Wall* extraWall) {
        const auto& walls = board.getWalls();
        
        // Check existing walls
        for (const auto& wall : walls) {
            if (isBlockedBy(wall, fromRow, fromCol, toRow, toCol)) {
                return true;
            }
        }
        
        // Check extra wall if provided
        if (extraWall) {
            if (isBlockedBy(*extraWall, fromRow, fromCol, toRow, toCol)) {
                return true;
            }
        }
        
        return false;
    }

    bool Rules::pathExists(const Board& board, int player, const Wall* extraWall) {
        Position startPos = board.getPawnPosition(player);
        int targetRow = (player == 0) ? (BOARD_SIZE - 1) : 0;
        
        // BFS
        std::queue<Position> q;
        q.push(startPos);
        
        std::vector<std::vector<bool>> visited(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
        visited[startPos.y][startPos.x] = true;
        
        // Directions: Up, Down, Left, Right
        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        
        while (!q.empty()) {
            Position curr = q.front();
            q.pop();
            
            if (curr.y == targetRow) {
                return true;
            }
            
            for (int i = 0; i < 4; ++i) {
                int nextRow = curr.y + dr[i];
                int nextCol = curr.x + dc[i];
                
                // Check bounds
                if (!Board::isInBounds(nextRow, nextCol)) {
                    continue;
                }
                
                if (visited[nextRow][nextCol]) {
                    continue;
                }
                
                // Check if blocked by walls (existing or extra)
                // Note: We use isPathBlockedByWall here, which logic is shared with isValidMove
                // We do NOT check isValidMove because that checks for pawn collisions, 
                // but path existence ignores pawns (they can move).
                if (isPathBlockedByWall(board, curr.y, curr.x, nextRow, nextCol, extraWall)) {
                    continue;
                }
                
                visited[nextRow][nextCol] = true;
                q.push({nextCol, nextRow});
            }
        }
        
        return false;
    }

} // namespace Quoridor
