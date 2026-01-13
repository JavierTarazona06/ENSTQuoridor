#include "model/AI.hpp"
#include "model/Rules.hpp"
#include "model/Pathfinder.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <limits>

namespace Quoridor {

    int AI::evaluate(const Board& board, int playerIndex) {
        int opponentIndex = (playerIndex == 0) ? 1 : 0;
        
        // Calculate distances to goal
        int myDist = Pathfinder::getShortestPathDistance(board, playerIndex);
        int oppDist = Pathfinder::getShortestPathDistance(board, opponentIndex);
        
        // Handle end game states
        if (myDist == 0) return std::numeric_limits<int>::max(); // I won
        if (oppDist == 0) return std::numeric_limits<int>::min(); // Opponent won
        
        // Handle blocked paths (should not happen if move generation is correct, but safe to handle)
        if (myDist == -1) return std::numeric_limits<int>::min() + 1; // I am blocked (avoid actual min to prevent overflow)
        if (oppDist == -1) return std::numeric_limits<int>::max() - 1; // Opponent blocked (I effectively win)
        
        // Base score: The closer I am relative to opponent, the better.
        // Weight: 10 points per step difference
        int score = (oppDist - myDist) * 10;
        
        // Bonus: Remaining walls
        // Having more walls is a strategic advantage (Relative to opponent)
        int myWalls = board.getWallsRemaining(playerIndex);
        int oppWalls = board.getWallsRemaining(opponentIndex);
        score += (myWalls - oppWalls) * 1; // 1 point per wall advantage
        
        return score;
    }

    Move AI::getBestMove(const Board& board, const State& state, Difficulty difficulty) {
        int player = state.getCurrentPlayer();
        
        switch (difficulty) {
            case Difficulty::Easy:
                return getRandomMove(board, player);
            case Difficulty::Normal:
                // Fallback to random for now
                return getRandomMove(board, player);
            case Difficulty::Hard:
                // Fallback to random for now
                return getRandomMove(board, player);
            default:
                return getRandomMove(board, player);
        }
    }

    Move AI::getRandomMove(const Board& board, int playerIndex) {
        std::vector<Position> pawnMoves = getValidPawnMoves(board, playerIndex);
        std::vector<Wall> wallMoves = getValidWallPlacements(board, playerIndex);

        // Setup random number generation
        static std::random_device rd;
        static std::mt19937 gen(rd());

        // Decide whether to move pawn or place wall
        // If no walls remaining, must move pawn
        bool canPlaceWall = !wallMoves.empty() && board.getWallsRemaining(playerIndex) > 0;
        
        // Simple heuristic: 80% chance to move pawn, 20% to place wall (if possible)
        // This makes the random bot slightly more aggressive/active
        std::uniform_int_distribution<> actionDist(0, 9);
        bool placeWall = canPlaceWall && (actionDist(gen) < 2); // 20% chance

        if (placeWall) {
            std::uniform_int_distribution<> wallDist(0, wallMoves.size() - 1);
            return Move::createWallMove(wallMoves[wallDist(gen)]);
        } else {
            if (pawnMoves.empty()) {
                // Should not happen if game is not over
                // Return a dummy move or handle error
                // For safety, return current position as a "stay" move (invalid but safe)
                return Move::createPawnMove(board.getPawnPosition(playerIndex));
            }
            std::uniform_int_distribution<> pawnDist(0, pawnMoves.size() - 1);
            return Move::createPawnMove(pawnMoves[pawnDist(gen)]);
        }
    }

    std::vector<Position> AI::getValidPawnMoves(const Board& board, int playerIndex) {
        std::vector<Position> validMoves;
        Position currentPos = board.getPawnPosition(playerIndex);

        // Check a 5x5 area around the pawn to cover all possible moves (including jumps)
        for (int r = currentPos.y - 2; r <= currentPos.y + 2; ++r) {
            for (int c = currentPos.x - 2; c <= currentPos.x + 2; ++c) {
                // Skip current position
                if (r == currentPos.y && c == currentPos.x) continue;

                if (Rules::isValidMove(board, playerIndex, currentPos.y, currentPos.x, r, c)) {
                    validMoves.push_back({c, r});
                }
            }
        }
        return validMoves;
    }

    std::vector<Wall> AI::getValidWallPlacements(const Board& board, int playerIndex) {
        std::vector<Wall> validWalls;
        
        // If no walls remaining, return empty list
        if (board.getWallsRemaining(playerIndex) <= 0) {
            return validWalls;
        }

        // Iterate through all possible wall positions
        // Board size is 9x9, so wall slots are 8x8 (0 to 7)
        for (int r = 0; r < BOARD_SIZE - 1; ++r) {
            for (int c = 0; c < BOARD_SIZE - 1; ++c) {
                // Check Horizontal
                Wall hWall = {{c, r}, Orientation::Horizontal};
                if (Rules::isValidWallPlacement(board, hWall, playerIndex)) {
                    validWalls.push_back(hWall);
                }

                // Check Vertical
                Wall vWall = {{c, r}, Orientation::Vertical};
                if (Rules::isValidWallPlacement(board, vWall, playerIndex)) {
                    validWalls.push_back(vWall);
                }
            }
        }
        return validWalls;
    }

} // namespace Quoridor
