#include "model/Pathfinder.hpp"
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <map> // For reconstructing path

namespace Quoridor {

    // ... (keep helper functions isBlockedBy and isPathBlocked same) ...
    // To save context space, I will rewrite the whole file but keeping helpers.

    static bool isBlockedBy(const Wall& wall, int fromRow, int fromCol, int toRow, int toCol) {
        bool movingVertical = (std::abs(toRow - fromRow) == 1);
        
        if (movingVertical) {
            if (wall.orientation == Orientation::Horizontal) {
                int splitRow = std::min(fromRow, toRow);
                if (wall.pos.y == splitRow) {
                    if (wall.pos.x == fromCol || wall.pos.x == fromCol - 1) return true;
                }
            }
        } else {
            if (wall.orientation == Orientation::Vertical) {
                int splitCol = std::min(fromCol, toCol);
                if (wall.pos.x == splitCol) {
                    if (wall.pos.y == fromRow || wall.pos.y == fromRow - 1) return true;
                }
            }
        }
        return false;
    }

    static bool isPathBlocked(const Board& board, int fromRow, int fromCol, int toRow, int toCol, const Wall* extraWall) {
        const auto& walls = board.getWalls();
        for (const auto& wall : walls) {
            if (isBlockedBy(wall, fromRow, fromCol, toRow, toCol)) return true;
        }
        if (extraWall) {
            if (isBlockedBy(*extraWall, fromRow, fromCol, toRow, toCol)) return true;
        }
        return false;
    }

    struct Node {
        Position pos;
        int f_score;
        bool operator>(const Node& other) const { return f_score > other.f_score; }
    };

    static int heuristic(const Position& pos, int targetRow) {
        return std::abs(pos.y - targetRow);
    }

    // Helper to reconstruct path
    static std::vector<Position> reconstructPath(const std::vector<std::vector<Position>>& cameFrom, Position current) {
        std::vector<Position> totalPath;
        totalPath.push_back(current);
        
        while (cameFrom[current.y][current.x].x != -1) {
            current = cameFrom[current.y][current.x];
            totalPath.push_back(current);
        }
        std::reverse(totalPath.begin(), totalPath.end());
        // Path includes start? Usually yes, or no?
        // Let's exclude start, so size == number of moves.
        if (!totalPath.empty()) totalPath.erase(totalPath.begin());
        return totalPath;
    }

    static std::vector<Position> solveAStar(const Board& board, int playerIndex, const Wall* extraWall) {
        Position startPos = board.getPawnPosition(playerIndex);
        int targetRow = (playerIndex == 0) ? (BOARD_SIZE - 1) : 0;

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
        openSet.push({startPos, heuristic(startPos, targetRow)});

        std::vector<std::vector<int>> g_score(BOARD_SIZE, std::vector<int>(BOARD_SIZE, std::numeric_limits<int>::max()));
        g_score[startPos.y][startPos.x] = 0;

        // Parent map for path reconstruction: initialize with {-1, -1}
        std::vector<std::vector<Position>> cameFrom(BOARD_SIZE, std::vector<Position>(BOARD_SIZE, {-1, -1}));

        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};

        while (!openSet.empty()) {
            Node current = openSet.top();
            openSet.pop();

            if (current.pos.y == targetRow) {
                return reconstructPath(cameFrom, current.pos);
            }

            if (current.f_score > g_score[current.pos.y][current.pos.x] + heuristic(current.pos, targetRow)) {
                continue;
            }

            for (int i = 0; i < 4; ++i) {
                int nextRow = current.pos.y + dr[i];
                int nextCol = current.pos.x + dc[i];

                if (!Board::isInBounds(nextRow, nextCol)) continue;
                if (isPathBlocked(board, current.pos.y, current.pos.x, nextRow, nextCol, extraWall)) continue;

                int tentative_g = g_score[current.pos.y][current.pos.x] + 1;

                if (tentative_g < g_score[nextRow][nextCol]) {
                    cameFrom[nextRow][nextCol] = current.pos;
                    g_score[nextRow][nextCol] = tentative_g;
                    int f = tentative_g + heuristic({nextCol, nextRow}, targetRow);
                    openSet.push({{nextCol, nextRow}, f});
                }
            }
        }

        return {}; // Return empty vector if no path
    }

    // =========================================================================
    // BFS Implementation for hasPathToGoal - More efficient for existence check
    // BFS is faster than A* here because:
    // 1. We only need to check if a path exists, not find the shortest one
    // 2. The search space is small (9x9 = 81 cells)
    // 3. BFS uses a simple queue (O(1) operations) vs A*'s priority queue (O(log n))
    // 4. No heuristic calculation overhead
    // =========================================================================

    static bool solveBFS(const Board& board, int playerIndex, const Wall* extraWall) {
        Position start = board.getPawnPosition(playerIndex);
        int targetRow = (playerIndex == 0) ? (BOARD_SIZE - 1) : 0;
        
        // Early termination: already at goal
        if (start.y == targetRow) return true;
        
        // Visited array
        std::vector<std::vector<bool>> visited(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
        
        // BFS queue
        std::queue<Position> queue;
        queue.push(start);
        visited[start.y][start.x] = true;
        
        // Direction vectors: up, down, left, right
        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        
        while (!queue.empty()) {
            Position current = queue.front();
            queue.pop();
            
            for (int i = 0; i < 4; ++i) {
                int nextRow = current.y + dr[i];
                int nextCol = current.x + dc[i];
                
                // Bounds check
                if (!Board::isInBounds(nextRow, nextCol)) continue;
                
                // Already visited
                if (visited[nextRow][nextCol]) continue;
                
                // Wall check (including hypothetical wall)
                if (isPathBlocked(board, current.y, current.x, nextRow, nextCol, extraWall)) continue;
                
                // Goal check - return immediately for early termination!
                if (nextRow == targetRow) return true;
                
                visited[nextRow][nextCol] = true;
                queue.push({nextCol, nextRow});
            }
        }
        
        return false; // No path found
    }

    bool Pathfinder::hasPathToGoal(const Board& board, int playerIndex, const Wall& hypotheticalWall) {
        return solveBFS(board, playerIndex, &hypotheticalWall);
    }

    bool Pathfinder::hasPathToGoal(const Board& board, int playerIndex) {
        return solveBFS(board, playerIndex, nullptr);
    }

    int Pathfinder::getShortestPathDistance(const Board& board, int playerIndex) {
        Position start = board.getPawnPosition(playerIndex);
        int targetRow = (playerIndex == 0) ? (BOARD_SIZE - 1) : 0;
        if (start.y == targetRow) return 0;

        auto path = solveAStar(board, playerIndex, nullptr);
        if (path.empty()) return -1;
        return static_cast<int>(path.size());
    }

    std::vector<Position> Pathfinder::getShortestPath(const Board& board, int playerIndex) {
        return solveAStar(board, playerIndex, nullptr);
    }

} // namespace Quoridor
