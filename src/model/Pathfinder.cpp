#include "model/Pathfinder.hpp"
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
namespace Quoridor {

    // Helper function to check if a specific wall blocks the move between two ADJACENT cells
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
                    // Check if wall spans the column we are in
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
                    // Check if wall spans the row we are in
                    if (wall.pos.y == fromRow || wall.pos.y == fromRow - 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    static bool isPathBlocked(const Board& board, int fromRow, int fromCol, int toRow, int toCol, const Wall* extraWall) {
        // Check existing walls
        const auto& walls = board.getWalls();
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

    bool Pathfinder::hasPathToGoal(const Board& board, int playerIndex, const Wall& hypotheticalWall) {
        Position startPos = board.getPawnPosition(playerIndex);
        
        // Player 0 (Top) -> Goal Row 8 (Bottom)
        // Player 1 (Bottom) -> Goal Row 0 (Top)
        int targetRow = (playerIndex == 0) ? (BOARD_SIZE - 1) : 0;

        // BFS Setup
        std::queue<Position> q;
        q.push(startPos);

        // Visited array
        std::vector<std::vector<bool>> visited(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
        visited[startPos.y][startPos.x] = true;

        // Directions: Up, Down, Left, Right
        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};

        while (!q.empty()) {
            Position curr = q.front();
            q.pop();

            // Check if we reached the target row
            if (curr.y == targetRow) {
                return true;
            }

            // Explore neighbors
            for (int i = 0; i < 4; ++i) {
                int nextRow = curr.y + dr[i];
                int nextCol = curr.x + dc[i];

                // 1. Check bounds
                if (!Board::isInBounds(nextRow, nextCol)) {
                    continue;
                }

                // 2. Check visited
                if (visited[nextRow][nextCol]) {
                    continue;
                }

                // 3. Check walls (existing + hypothetical)
                if (isPathBlocked(board, curr.y, curr.x, nextRow, nextCol, &hypotheticalWall)) {
                    continue;
                }

                // Mark as visited and add to queue
                visited[nextRow][nextCol] = true;
                q.push({nextCol, nextRow});
            }
        }

        return false;
    }

    bool Pathfinder::hasPathToGoal(const Board& board, int playerIndex) {
        // Just call the main function with a dummy wall that is out of bounds/invalid so it won't block anything
        // Or we could pass nullptr if we changed the signature to accept pointer.
        // But since we take const ref, let's create a dummy wall.
        // A wall at (-10, -10) should not block anything on the board.
        Wall dummyWall = {{-10, -10}, Orientation::Horizontal};
        return hasPathToGoal(board, playerIndex, dummyWall);
    }

} // namespace Quoridor
