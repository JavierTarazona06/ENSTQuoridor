#pragma once

#include "model/Board.hpp"
#include "model/State.hpp"
#include <vector>
#include <optional>

namespace Quoridor {

    enum class Difficulty {
        Easy,   // Random
        Normal, // Minimax Depth 2
        Hard,   // Minimax Depth 3
        Hell    // Minimax Depth 5 (WARNING: Can be slow in complex board states)
    };

    struct Move {
        enum class Type { PawnMove, WallPlacement };
        Type type;
        Position pawnDest; // Valid if type == PawnMove
        Wall wall;         // Valid if type == WallPlacement
        
        // Helper constructors
        static Move createPawnMove(Position pos) {
            Move m;
            m.type = Type::PawnMove;
            m.pawnDest = pos;
            // Initialize wall with dummy values to avoid warnings
            m.wall = {{-1, -1}, Orientation::Horizontal}; 
            return m;
        }
        
        static Move createWallMove(Wall w) {
            Move m;
            m.type = Type::WallPlacement;
            m.wall = w;
            // Initialize pawnDest with dummy values
            m.pawnDest = {-1, -1};
            return m;
        }

        bool isPawnMove() const { return type == Type::PawnMove; }
        bool isWallPlacement() const { return type == Type::WallPlacement; }
        
        bool operator==(const Move& other) const {
            if (type != other.type) return false;
            if (type == Type::PawnMove) return pawnDest == other.pawnDest;
            return wall == other.wall;
        }
    };

    class AI {
    public:
        AI() = default;
        ~AI() = default;

        /**
         * @brief Get the best move for the current player based on difficulty.
         * @param board The current board state
         * @param state The current game state (to know current player)
         * @param difficulty The desired difficulty level
         * @return A Move structure containing either a pawn move or wall placement
         */
        Move getBestMove(const Board& board, const State& state, Difficulty difficulty);

        /**
         * @brief Evaluates the current board state from the perspective of the given player.
         * @param board The current board state.
         * @param playerIndex The player for whom we are evaluating (0 or 1).
         * @return A score integer. Higher is better for playerIndex.
         */
        int evaluate(const Board& board, int playerIndex);

    private:
        /**
         * @brief Strategy for Easy difficulty: Random valid move
         */
        Move getRandomMove(const Board& board, int playerIndex);
        
        /**
         * @brief Generate all valid pawn moves for a player
         */
        std::vector<Position> getValidPawnMoves(const Board& board, int playerIndex);
        
        /**
         * @brief Generate all valid wall placements for a player
         */
        std::vector<Wall> getValidWallPlacements(const Board& board, int playerIndex);

        /**
         * @brief Minimax algorithm with Alpha-Beta pruning
         * 
         * @param board Current board state
         * @param depth Current depth remaining
         * @param alpha Best value maximizing player can guarantee
         * @param beta Best value minimizing player can guarantee
         * @param maximizingPlayer True if current turn is for the AI (maximizer)
         * @param playerIndex The AI's player index (constant across recursion)
         * @return Best score found
         */
        int minimax(Board board, int depth, int alpha, int beta, bool maximizingPlayer, int playerIndex);
        
        // Helper to collect all moves (pawn + walls)
        std::vector<Move> getAllValidMoves(const Board& board, int currentPlayerIndex);
    };

} // namespace Quoridor
