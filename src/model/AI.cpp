#include "model/AI.hpp"
#include "model/Rules.hpp"
#include "model/Pathfinder.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <limits>
#include <set>

namespace Quoridor {

    // Helper for set uniqueness
    bool operator<(const Wall& a, const Wall& b) {
        if (a.pos.x != b.pos.x) return a.pos.x < b.pos.x;
        if (a.pos.y != b.pos.y) return a.pos.y < b.pos.y;
        return a.orientation < b.orientation;
    }

    // ============================================================================
    // Evaluation Function Weights (tunable parameters)
    // ============================================================================
    namespace EvalWeights {
        constexpr int DISTANCE_DIFF = 10;       // Base weight for path distance difference
        constexpr int WALL_ADVANTAGE = 2;       // Weight per wall advantage
        constexpr int MOBILITY = 3;             // Weight for movement freedom
        constexpr int ENDGAME_BONUS = 15;       // Extra weight when close to winning
        constexpr int WALL_EFFECTIVENESS = 5;   // Weight for wall blocking value
    }

    int AI::evaluate(const Board& board, int playerIndex) {
        int opponentIndex = (playerIndex == 0) ? 1 : 0;
        
        // Calculate distances to goal
        int myDist = Pathfinder::getShortestPathDistance(board, playerIndex);
        int oppDist = Pathfinder::getShortestPathDistance(board, opponentIndex);
        
        // ===== Terminal State Detection =====
        if (myDist == 0) return std::numeric_limits<int>::max(); // I won
        if (oppDist == 0) return std::numeric_limits<int>::min(); // Opponent won
        
        // Handle blocked paths
        if (myDist == -1) return std::numeric_limits<int>::min() + 1; // I am blocked
        if (oppDist == -1) return std::numeric_limits<int>::max() - 1; // Opponent blocked
        
        int score = 0;
        
        // ===== Factor 1: Path Distance Difference (Most Important) =====
        // The closer I am relative to opponent, the better
        score += (oppDist - myDist) * EvalWeights::DISTANCE_DIFF;
        
        // ===== Factor 2: Endgame Bonus (Non-linear reward near victory) =====
        // When close to winning, prioritize advancing over other tactics
        if (myDist <= 3) {
            // Exponential bonus: closer = much higher bonus
            score += (4 - myDist) * (4 - myDist) * EvalWeights::ENDGAME_BONUS;
        }
        // When opponent is close to winning, increase urgency to block
        if (oppDist <= 3) {
            // Penalty increases as opponent approaches goal
            score -= (4 - oppDist) * (4 - oppDist) * EvalWeights::ENDGAME_BONUS;
        }
        
        // ===== Factor 3: Wall Advantage =====
        // Having more walls provides strategic flexibility
        int myWalls = board.getWallsRemaining(playerIndex);
        int oppWalls = board.getWallsRemaining(opponentIndex);
        score += (myWalls - oppWalls) * EvalWeights::WALL_ADVANTAGE;
        
        // ===== Factor 4: Mobility (Movement Freedom) =====
        // More movement options = better tactical flexibility
        // Count valid pawn moves for both players
        int myMobility = 0;
        int oppMobility = 0;
        
        Position myPos = board.getPawnPosition(playerIndex);
        Position oppPos = board.getPawnPosition(opponentIndex);
        
        // Check adjacent cells for valid moves (simplified mobility check)
        const int dx[] = {0, 0, 1, -1};
        const int dy[] = {1, -1, 0, 0};
        
        for (int i = 0; i < 4; ++i) {
            int newX = myPos.x + dx[i];
            int newY = myPos.y + dy[i];
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE) {
                if (Rules::isValidMove(board, playerIndex, myPos.y, myPos.x, newY, newX)) {
                    myMobility++;
                }
            }
            
            newX = oppPos.x + dx[i];
            newY = oppPos.y + dy[i];
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE) {
                if (Rules::isValidMove(board, opponentIndex, oppPos.y, oppPos.x, newY, newX)) {
                    oppMobility++;
                }
            }
        }
        
        score += (myMobility - oppMobility) * EvalWeights::MOBILITY;
        
        // ===== Factor 5: Wall Effectiveness (Strategic Wall Value) =====
        // Reward if opponent's path has been lengthened by our walls
        // Compare opponent's current distance to their theoretical minimum (without walls blocking)
        // A rough heuristic: opponent's base distance is their row distance to goal
        int oppBaseDistance = (opponentIndex == 0) ? (BOARD_SIZE - 1 - oppPos.y) : oppPos.y;
        int wallEffectiveness = oppDist - oppBaseDistance;
        if (wallEffectiveness > 0) {
            score += wallEffectiveness * EvalWeights::WALL_EFFECTIVENESS;
        }
        
        // Similarly, penalize if our path is blocked by opponent's walls
        int myBaseDistance = (playerIndex == 0) ? (BOARD_SIZE - 1 - myPos.y) : myPos.y;
        int oppWallEffectiveness = myDist - myBaseDistance;
        if (oppWallEffectiveness > 0) {
            score -= oppWallEffectiveness * EvalWeights::WALL_EFFECTIVENESS;
        }
        
        return score;
    }

    Move AI::getBestMove(const Board& board, const State& state, Difficulty difficulty) {
        int player = state.getCurrentPlayer();
        
        if (difficulty == Difficulty::Easy) {
            return getRandomMove(board, player);
        }

        // Minimax configuration
        int depth = 1; // Default Normal
        if (difficulty == Difficulty::Hard) depth = 2;
        if (difficulty == Difficulty::Hell) depth = 4;
        
        int bestScore = std::numeric_limits<int>::min();
        Move bestMove; // Default invalid move
        bool moveFound = false;

        std::vector<Move> moves = getAllValidMoves(board, player);
        
        // Simple move ordering optimization: Consider pawn moves first (usually better/faster to verify)
        // They are already added first in getAllValidMoves.

        for (const auto& move : moves) {
            Board nextBoard = board;
            if (move.isPawnMove()) {
                nextBoard.movePawn(player, move.pawnDest);
            } else {
                nextBoard.placeWall(move.wall, player);
            }
            
            // Call minimax for the next level (opponent's turn -> minimizing)
            // Pass 'player' as the reference 'playerIndex' for evaluation
            int score = minimax(nextBoard, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, player);
            
            // Debug output
            /*
            if (move.isPawnMove()) {
                std::cout << "Move Pawn to " << move.pawnDest.x << "," << move.pawnDest.y << " Score: " << score << std::endl;
            } else {
                std::cout << "Place Wall at " << move.wall.pos.x << "," << move.wall.pos.y << " Score: " << score << std::endl;
            }
            */

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
                moveFound = true;
            }
        }
        
        if (!moveFound && !moves.empty()) {
            return moves[0]; // Fallback
        }
        
        return bestMove;
    }

    int AI::minimax(Board board, int depth, int alpha, int beta, bool maximizingPlayer, int playerIndex) {
        // Base case: leaf node or game over
        // Check for immediate win/loss first to avoid evaluating non-terminal states as terminal
        // Actually evaluate() handles win/loss logic (INT_MAX/INT_MIN)
        
        if (depth == 0) {
            return evaluate(board, playerIndex);
        }
        
        // Check for game over via evaluation (if score is MAX/MIN, it's terminal)
        int currentEval = evaluate(board, playerIndex);
        if (currentEval == std::numeric_limits<int>::max() || currentEval == std::numeric_limits<int>::min()) {
            return currentEval;
        }

        int currentPlayerIndex = maximizingPlayer ? playerIndex : ((playerIndex == 0) ? 1 : 0);
        std::vector<Move> moves = getAllValidMoves(board, currentPlayerIndex);

        if (moves.empty()) {
            return currentEval; // Stalemate or trapped? Should return loss usually.
        }

        if (maximizingPlayer) {
            int maxEval = std::numeric_limits<int>::min();
            for (const auto& move : moves) {
                Board nextBoard = board;
                if (move.isPawnMove()) {
                    nextBoard.movePawn(currentPlayerIndex, move.pawnDest);
                } else {
                    nextBoard.placeWall(move.wall, currentPlayerIndex);
                }
                
                int eval = minimax(nextBoard, depth - 1, alpha, beta, false, playerIndex);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break; // Beta prune
                }
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (const auto& move : moves) {
                Board nextBoard = board;
                if (move.isPawnMove()) {
                    nextBoard.movePawn(currentPlayerIndex, move.pawnDest);
                } else {
                    nextBoard.placeWall(move.wall, currentPlayerIndex);
                }
                
                int eval = minimax(nextBoard, depth - 1, alpha, beta, true, playerIndex);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break; // Alpha prune
                }
            }
            return minEval;
        }
    }

    std::vector<Move> AI::getAllValidMoves(const Board& board, int playerIndex) {
        std::vector<Move> moves;
        
        // Pawn moves
        auto pMoves = getValidPawnMoves(board, playerIndex);
        for (const auto& p : pMoves) {
            moves.push_back(Move::createPawnMove(p));
        }
        
        // Wall moves optimization: Only consider relevant walls
        if (board.getWallsRemaining(playerIndex) > 0) {
            // Strategy: Block opponent's shortest path & Box opponent
            int opponentIndex = (playerIndex == 0) ? 1 : 0;
            
            std::set<Wall> relevantWalls; // Use set to avoid duplicates

            // 1. Walls blocking the opponent's shortest path
            std::vector<Position> path = Pathfinder::getShortestPath(board, opponentIndex);
            if (!path.empty()) {
                Position current = board.getPawnPosition(opponentIndex);
                for (const auto& next : path) {
                    // Moving Vertical (row change)
                    if (current.x == next.x) {
                        int r = std::min(current.y, next.y);
                        // Horizontal walls that block vertical movement at column 'current.x'
                        relevantWalls.insert({{current.x, r}, Orientation::Horizontal});
                        relevantWalls.insert({{current.x - 1, r}, Orientation::Horizontal});
                    } 
                    // Moving Horizontal (col change)
                    else if (current.y == next.y) {
                        int c = std::min(current.x, next.x);
                        // Vertical walls that block horizontal movement at row 'current.y'
                        relevantWalls.insert({{c, current.y}, Orientation::Vertical});
                        relevantWalls.insert({{c, current.y - 1}, Orientation::Vertical});
                    }
                    current = next;
                }
            }

            // 2. Walls near opponent (aggressive/defensive box)
            // Consider immediate vicinity to ensure we don't miss tactical blocks not on current shortest path
            Position oppPos = board.getPawnPosition(opponentIndex);
            for (int r = oppPos.y - 1; r <= oppPos.y; ++r) {
                for (int c = oppPos.x - 1; c <= oppPos.x; ++c) {
                    relevantWalls.insert({{c, r}, Orientation::Horizontal});
                    relevantWalls.insert({{c, r}, Orientation::Vertical});
                }
            }

            // 3. Walls near self (defensive against blocking?)
            // Maybe less critical for Hell mode speed, skip for now.

            // Validate and add unique walls
            for (const auto& w : relevantWalls) {
                if (Rules::isValidWallPlacement(board, w, playerIndex)) {
                    moves.push_back(Move::createWallMove(w));
                }
            }
        }
        
        return moves;
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
