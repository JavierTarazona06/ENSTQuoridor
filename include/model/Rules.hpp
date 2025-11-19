#pragma once

#include "model/Board.hpp"

namespace Quoridor {

    class Rules {
    public:
        /**
         * @brief Checks if a move is valid according to basic rules (orthogonal, bounds, empty).
         * @param board The current board state.
         * @param player The player index attempting the move (0 or 1).
         * @param fromRow Starting row.
         * @param fromCol Starting column.
         * @param toRow Target row.
         * @param toCol Target column.
         * @return true if valid, false otherwise.
         */
        static bool isValidMove(const Board& board, int player, int fromRow, int fromCol, int toRow, int toCol);

        /**
         * @brief Checks if the specified player has won.
         * @param board The current board state.
         * @param player The player index.
         * @return true if the player has reached their objective row.
         */
        static bool checkVictory(const Board& board, int player);

    private:
        // Helper to check if a wall blocks the path between two adjacent cells
        static bool isPathBlockedByWall(const Board& board, int fromRow, int fromCol, int toRow, int toCol);
    };

} // namespace Quoridor
