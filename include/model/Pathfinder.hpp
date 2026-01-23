#pragma once

#include "model/Board.hpp"
#include <vector>

namespace Quoridor {

    class Pathfinder {
    public:
        /**
         * @brief Checks if a path exists for the player to reach their goal line.
         */
        static bool hasPathToGoal(const Board& board, int playerIndex, const Wall& hypotheticalWall);

        /**
         * @brief Checks if a path exists for the player to reach their goal line (without hypothetical wall).
         */
        static bool hasPathToGoal(const Board& board, int playerIndex);

        /**
         * @brief Calculates the shortest path distance (number of moves) to the goal line using A*.
         * @return The number of moves to reach the goal, or -1 if no path exists.
         */
        static int getShortestPathDistance(const Board& board, int playerIndex);

        /**
         * @brief Returns the shortest path as a sequence of positions.
         * @return Vector of positions from start (exclusive) to goal (inclusive). Empty if no path.
         */
        static std::vector<Position> getShortestPath(const Board& board, int playerIndex);
    };

} // namespace Quoridor
