#include "model/Rules.hpp"
#include <cmath>
#include <queue>
#include <set>

namespace Quoridor {

    // Helper to check if an opponent is at a specific position
    static bool isOpponentAt(const Board& board, int currentPlayer, int row, int col) {
        for (int p = 0; p < NUM_PLAYERS; ++p) {
            if (p == currentPlayer) continue;
            Position pos = board.getPawnPosition(p);
            if (pos.x == col && pos.y == row) {
                return true;
            }
        }
        return false;
    }

    bool Rules::isValidMove(const Board& board, int player, int fromRow, int fromCol, int toRow, int toCol) {
        // 1. Check bounds
        if (!Board::isInBounds(toRow, toCol)) {
            return false;
        }

        // 2. Check if target cell is occupied
        for (int p = 0; p < NUM_PLAYERS; ++p) {
            Position pos = board.getPawnPosition(p);
            if (pos.x == toCol && pos.y == toRow) {
                return false; // Target occupied
            }
        }

        int dRow = std::abs(toRow - fromRow);
        int dCol = std::abs(toCol - fromCol);
        int dist = dRow + dCol;

        // Simple orthogonal move (1 step)
        if (dist == 1) {
            return !isPathBlockedByWall(board, fromRow, fromCol, toRow, toCol);
        }
        
        // Jump moves (distance 2, either straight 2+0 or diagonal 1+1)
        if (dist == 2) {
            // Straight Jump
            if ((dRow == 2 && dCol == 0) || (dRow == 0 && dCol == 2)) {
                int midRow = (fromRow + toRow) / 2;
                int midCol = (fromCol + toCol) / 2;
                
                // Must jump over an opponent
                if (!isOpponentAt(board, player, midRow, midCol)) {
                    return false;
                }
                
                // Check walls: from->mid and mid->to
                if (isPathBlockedByWall(board, fromRow, fromCol, midRow, midCol)) return false;
                if (isPathBlockedByWall(board, midRow, midCol, toRow, toCol)) return false;
                
                return true;
            }
            // Diagonal Jump
            else if (dRow == 1 && dCol == 1) {
                // We can jump diagonally ONLY if the straight jump is blocked
                
                // Option 1: Opponent is horizontally adjacent (at fromRow, toCol)
                if (isOpponentAt(board, player, fromRow, toCol)) {
                    // Check if the straight jump behind this opponent is blocked
                    // Straight jump destination would be (fromRow, toCol + (toCol - fromCol))
                    int jumpCol = toCol + (toCol - fromCol);
                    bool straightBlocked = false;
                    
                    if (!Board::isInBounds(fromRow, jumpCol)) {
                        straightBlocked = true; // Blocked by board edge
                    } else if (isPathBlockedByWall(board, fromRow, toCol, fromRow, jumpCol)) {
                        straightBlocked = true; // Blocked by wall behind opponent
                    } else {
                        // Check if jump destination is occupied (unlikely in 2p if opponent is at mid)
                        for(int p=0; p<NUM_PLAYERS; ++p) {
                            Position pos = board.getPawnPosition(p);
                            if(pos.x == jumpCol && pos.y == fromRow) straightBlocked = true;
                        }
                    }

                    if (straightBlocked) {
                         // Valid diagonal if path not blocked by walls
                         // Path: (fromRow, fromCol) -> (fromRow, toCol) -> (toRow, toCol)
                         if (!isPathBlockedByWall(board, fromRow, fromCol, fromRow, toCol) && 
                             !isPathBlockedByWall(board, fromRow, toCol, toRow, toCol)) {
                             return true;
                         }
                    }
                }
                
                // Option 2: Opponent is vertically adjacent (at toRow, fromCol)
                if (isOpponentAt(board, player, toRow, fromCol)) {
                    // Check if straight jump behind opponent is blocked
                    // Straight jump destination would be (toRow + (toRow - fromRow), fromCol)
                    int jumpRow = toRow + (toRow - fromRow);
                    bool straightBlocked = false;
                    
                    if (!Board::isInBounds(jumpRow, fromCol)) {
                        straightBlocked = true; // Blocked by board edge
                    } else if (isPathBlockedByWall(board, toRow, fromCol, jumpRow, fromCol)) {
                        straightBlocked = true; // Blocked by wall behind opponent
                    } else {
                         for(int p=0; p<NUM_PLAYERS; ++p) {
                            Position pos = board.getPawnPosition(p);
                            if(pos.x == fromCol && pos.y == jumpRow) straightBlocked = true;
                        }
                    }

                    if (straightBlocked) {
                        // Valid diagonal if path not blocked by walls
                        // Path: (fromRow, fromCol) -> (toRow, fromCol) -> (toRow, toCol)
                         if (!isPathBlockedByWall(board, fromRow, fromCol, toRow, fromCol) && 
                             !isPathBlockedByWall(board, toRow, fromCol, toRow, toCol)) {
                             return true;
                         }
                    }
                }
            }
        }

        return false;
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
