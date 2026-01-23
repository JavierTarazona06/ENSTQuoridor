#pragma once

#include "model/Board.hpp"
#include "model/State.hpp"
#include <vector>
#include <optional>

namespace Quoridor {

    /**
     * @brief AI difficulty levels with balanced gameplay
     * 
     * Each level uses different search depth and noise injection:
     * - Easy:   Random moves (no strategy)
     * - Normal: Depth 1, noise=25 (makes occasional mistakes, beatable)
     * - Hard:   Depth 2, noise=8  (rarely makes mistakes, challenging)
     * - Hell:   Depth 4, noise=0  (perfect calculation, very difficult)
     */
    enum class Difficulty {
        Easy,   // Random moves
        Normal, // Depth 1 + noise (beatable by average player)
        Hard,   // Depth 2 + low noise (challenging)
        Hell    // Depth 4, no noise (expert level)
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
         * @param noiseLevel Random noise to inject for difficulty balancing
         * @return Best score found
         */
        int minimax(Board board, int depth, int alpha, int beta, bool maximizingPlayer, int playerIndex, int noiseLevel = 0);
        
        /**
         * @brief Evaluate with random noise injection for difficulty balancing
         * @param board Board to evaluate
         * @param playerIndex Player index (0 or 1)
         * @param noiseLevel Amount of random noise to add (0 = perfect)
         * @return Score with noise applied (terminal states unaffected)
         */
        int evaluateWithNoise(const Board& board, int playerIndex, int noiseLevel);
        
        // Helper to collect all moves (pawn + walls)
        std::vector<Move> getAllValidMoves(const Board& board, int currentPlayerIndex);
    };

} // namespace Quoridor
