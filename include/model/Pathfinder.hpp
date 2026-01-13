#pragma once

#include "model/Board.hpp"

namespace Quoridor {

    class Pathfinder {
    public:
        /**
         * @brief Checks if a path exists for the player to reach their goal line.
         * 
         * @param board The current board state.
         * @param playerIndex The index of the player (0 or 1).
         * @param hypotheticalWall A wall that is hypothetically placed on the board.
         * @return true if at least one path exists to the goal row, false otherwise.
         */
        static bool hasPathToGoal(const Board& board, int playerIndex, const Wall& hypotheticalWall);

        /**
         * @brief Checks if a path exists for the player to reach their goal line (without hypothetical wall).
         * 
         * @param board The current board state.
         * @param playerIndex The index of the player (0 or 1).
         * @return true if at least one path exists to the goal row, false otherwise.
         */
        static bool hasPathToGoal(const Board& board, int playerIndex);
    };

} // namespace Quoridor
