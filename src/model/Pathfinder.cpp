#include "model/Pathfinder.hpp"
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <set>

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

    // A* Node structure
    struct Node {
        Position pos;
        int f_score; // g + h
        
        // Priority queue orders largest element first, so we want smallest f_score to be "largest" priority
        // Wait, standard std::greater makes smallest element appear at top.
        // So we need operator> to mean "larger cost".
        bool operator>(const Node& other) const {
            return f_score > other.f_score;
        }
    };

    static int heuristic(const Position& pos, int targetRow) {
        // Manhattan distance to the target row (we only care about vertical distance)
        // Since we can move horizontally freely to find a gap, the minimum steps required is just |y - target|
        // Horizontal distance is 0 because the target is the entire row.
        return std::abs(pos.y - targetRow);
    }

    static int solveAStar(const Board& board, int playerIndex, const Wall* extraWall) {
        Position startPos = board.getPawnPosition(playerIndex);
        int targetRow = (playerIndex == 0) ? (BOARD_SIZE - 1) : 0;

        // Min-priority queue
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
        openSet.push({startPos, heuristic(startPos, targetRow)});

        // g_score map: cost from start to node
        // Initialize with infinity
        std::vector<std::vector<int>> g_score(BOARD_SIZE, std::vector<int>(BOARD_SIZE, std::numeric_limits<int>::max()));
        g_score[startPos.y][startPos.x] = 0;

        // Directions: Up, Down, Left, Right
        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};

        while (!openSet.empty()) {
            Node current = openSet.top();
            openSet.pop();

            // Check if we reached the target row
            if (current.pos.y == targetRow) {
                // Return actual distance (g_score)
                return g_score[current.pos.y][current.pos.x];
            }

            // Optimization: If we found a shorter path to this node already, skip
            if (current.f_score > g_score[current.pos.y][current.pos.x] + heuristic(current.pos, targetRow)) {
                // Actually f = g + h, so g = f - h. 
                // Easier: just check if g_score is better than what current implies?
                // Standard A*: pop node.
                continue;
            }

            // Explore neighbors
            for (int i = 0; i < 4; ++i) {
                int nextRow = current.pos.y + dr[i];
                int nextCol = current.pos.x + dc[i];

                // 1. Check bounds
                if (!Board::isInBounds(nextRow, nextCol)) {
                    continue;
                }

                // 2. Check walls
                if (isPathBlocked(board, current.pos.y, current.pos.x, nextRow, nextCol, extraWall)) {
                    continue;
                }

                // Tentative g_score
                int tentative_g = g_score[current.pos.y][current.pos.x] + 1;

                // If path is better
                if (tentative_g < g_score[nextRow][nextCol]) {
                    g_score[nextRow][nextCol] = tentative_g;
                    int f = tentative_g + heuristic({nextCol, nextRow}, targetRow);
                    openSet.push({{nextCol, nextRow}, f});
                }
            }
        }

        return -1; // No path found
    }

    bool Pathfinder::hasPathToGoal(const Board& board, int playerIndex, const Wall& hypotheticalWall) {
        return solveAStar(board, playerIndex, &hypotheticalWall) != -1;
    }

    bool Pathfinder::hasPathToGoal(const Board& board, int playerIndex) {
        return solveAStar(board, playerIndex, nullptr) != -1;
    }

    int Pathfinder::getShortestPathDistance(const Board& board, int playerIndex) {
        return solveAStar(board, playerIndex, nullptr);
    }

} // namespace Quoridor
