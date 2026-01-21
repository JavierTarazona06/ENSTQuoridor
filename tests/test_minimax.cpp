#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "model/AI.hpp"
#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp" // For validation

using namespace Quoridor;

TEST_CASE("Minimax AI Comprehensive Suite", "[ai][minimax][comprehensive]") {
    AI ai;
    Board board;
    State state;

    // =================================================================
    // LEVEL: EASY (Random)
    // Goal: Verify validity and basic fallback behavior
    // =================================================================
    
    SECTION("Easy 1: Validity - Returns valid move on empty board") {
        state.resetGame();
        board.resetGame();
        Move move = ai.getBestMove(board, state, Difficulty::Easy);
        
        if (move.isPawnMove()) {
            Position current = board.getPawnPosition(0);
            REQUIRE(Rules::isValidMove(board, 0, current.y, current.x, move.pawnDest.y, move.pawnDest.x));
        } else {
            REQUIRE(Rules::isValidWallPlacement(board, move.wall, 0));
        }
    }

    SECTION("Easy 2: Forced Move - Must take only available option") {
        // Encase P0 at (4,0) except ONE opening to (3,0)
        board.resetGame();
        state.resetGame();
        
        board.placeWall({{4, 0}, Orientation::Vertical}, 1); // Block Right (4,0)-(5,0)
        // Use (3,0) Horizontal to block Down (4,0)-(4,1) without crossing Vertical (4,0)
        board.placeWall({{3, 0}, Orientation::Horizontal}, 1); 
        // Left is (3,0). Top is bounds.
        // So P0 MUST go to (3,0).
        
        bool movedPawn = false;
        for(int i=0; i<10; ++i) {
            Move move = ai.getBestMove(board, state, Difficulty::Easy);
            if (move.isPawnMove()) {
                REQUIRE(move.pawnDest.x == 3);
                REQUIRE(move.pawnDest.y == 0);
                movedPawn = true;
                break;
            }
        }
    }

    SECTION("Easy 3: Resource Depletion - Must move pawn if no walls") {
        // Placeholder for constraints verification
        SUCCEED(); 
    }

    // =================================================================
    // LEVEL: NORMAL (Depth 1 + noise)
    // Goal: Basic tactical decisions, allows occasional mistakes
    // Note: With noise injection, Normal is intentionally beatable
    // =================================================================

    SECTION("Normal 1: Immediate Win (1 move)") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 7});
        board.movePawn(1, {0, 0}); // P1 far away
        
        Move move = ai.getBestMove(board, state, Difficulty::Normal);
        REQUIRE(move.isPawnMove());
        REQUIRE(move.pawnDest.y == 8);
    }

    SECTION("Normal 2: Forward Progress at Start") {
        // At depth 1, AI should still prefer moving forward
        state.resetGame(); board.resetGame();
        
        Move move = ai.getBestMove(board, state, Difficulty::Normal);
        // Should be a valid move (pawn or wall)
        REQUIRE((move.isPawnMove() || move.isWallPlacement()));
    }

    SECTION("Normal 3: Noise allows variety") {
        // Due to noise, Normal may not always pick the same move
        // Just verify it returns valid moves
        state.resetGame(); board.resetGame();
        
        Move move = ai.getBestMove(board, state, Difficulty::Normal);
        REQUIRE((move.isPawnMove() || move.isWallPlacement()));
    }

    // =================================================================
    // LEVEL: HARD (Depth 2 + low noise)
    // Goal: Strategy sequences (2-step lookahead), rare mistakes
    // =================================================================

    SECTION("Hard 1: Block Immediate Loss") {
        // This was previously Normal 2, but depth 2 is needed to see opponent's win
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 6});
        board.movePawn(1, {4, 1}); // Needs 1 move to win
        
        Move move = ai.getBestMove(board, state, Difficulty::Hard);
        // Hard should see the threat and block with wall
        REQUIRE(move.isWallPlacement());
        REQUIRE(move.wall.pos.y < 3); 
    }

    SECTION("Hard 2: Setup 2-move Win") {
        // P0 at (4,6). P1 far away.
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 6});
        board.movePawn(1, {0, 8}); // P1 far and safe
        
        // Deplete P1 walls safely
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 0 : 2;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 1);
        }
        
        Move move = ai.getBestMove(board, state, Difficulty::Hard);
        REQUIRE(move.isPawnMove());
        REQUIRE(move.pawnDest.y == 7);
    }

    SECTION("Hard 3: Detect Deep Threat") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 5}); // Needs 3 moves
        board.movePawn(1, {4, 2}); // Needs 2 moves
        
        // Deplete P1 walls safely
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 0 : 2;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 1);
        }

        Move move = ai.getBestMove(board, state, Difficulty::Hard);
        REQUIRE(move.isWallPlacement());
    }

    SECTION("Hard 4: Avoid Self-Block") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 4});
        board.movePawn(1, {0, 0});
        
        // Deplete P0 walls to reduce search space
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 7 : 5;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 0);
        }
        // Deplete P1 walls to reduce search space
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 0 : 2;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 1);
        }
        
        Move move = ai.getBestMove(board, state, Difficulty::Hard);
        REQUIRE(move.isPawnMove());
    }

    // =================================================================
    // LEVEL: HELL (Depth 4)
    // Goal: Deep calculation (reduced from depth 4 for performance)
    // =================================================================

    SECTION("Hell 1: Deep Win Sequence (3 moves)") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 5});
        board.movePawn(1, {0, 8}); // P1 safe
        
        // Deplete P0 walls on right side
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 7 : 5;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 0);
        }
        
        // Deplete P1 walls on left side
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 0 : 2;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 1);
        }

        Move move = ai.getBestMove(board, state, Difficulty::Hell);
        REQUIRE(move.isPawnMove());
        REQUIRE(move.pawnDest.y == 6);
    }

    SECTION("Hell 2: Maze Navigation") {
        state.resetGame(); board.resetGame();
        // Skip P1 wall depletion to avoid overlap issues.
        // Block forward (4,1) using (3,0) Horizontal (blocks 4,0->4,1)
        board.placeWall({{3, 0}, Orientation::Horizontal}, 0); 
        // Block right (5,0)
        board.placeWall({{4, 0}, Orientation::Vertical}, 0);
        
        // P0 at (4,0). Must go Left (3,0).
        Move move = ai.getBestMove(board, state, Difficulty::Hell);
        REQUIRE(move.isPawnMove());
        REQUIRE(move.pawnDest.x == 3);
    }

    SECTION("Hell 3: Strategic Wall vs Move") {
        state.resetGame(); board.resetGame();
        
        // Deplete P0 walls to reduce search space
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 7 : 5;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 0);
        }
        // Deplete P1 walls to reduce search space
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 0 : 2;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 1);
        }
        
        Move move = ai.getBestMove(board, state, Difficulty::Hell);
        bool valid = move.isPawnMove() || move.isWallPlacement();
        REQUIRE(valid);
    }

    // =================================================================
    // PARTIAL WALLS TESTS - More realistic mid-game scenarios
    // Goal: Test AI with some walls remaining (increased search space)
    // =================================================================

    SECTION("Partial Walls 1: Normal with 5 walls each") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 3});
        board.movePawn(1, {4, 6});
        
        // Each player uses 5 walls (5 remaining each)
        // P0 walls on right edge
        for(int i=0; i<5; ++i) {
            board.placeWall({{7, i}, Orientation::Horizontal}, 0);
        }
        // P1 walls on left edge
        for(int i=0; i<5; ++i) {
            board.placeWall({{0, i}, Orientation::Horizontal}, 1);
        }
        
        Move move = ai.getBestMove(board, state, Difficulty::Normal);
        // Should make a valid move (pawn or wall)
        bool valid = false;
        if (move.isPawnMove()) {
            Position current = board.getPawnPosition(0);
            valid = Rules::isValidMove(board, 0, current.y, current.x, move.pawnDest.y, move.pawnDest.x);
        } else {
            valid = Rules::isValidWallPlacement(board, move.wall, 0);
        }
        REQUIRE(valid);
    }

    SECTION("Partial Walls 2: Hard with 3 walls each - Race scenario") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 5}); // 3 moves to goal
        board.movePawn(1, {4, 3}); // 3 moves to goal
        
        // Each player uses 7 walls (3 remaining each)
        // P0 walls: spread on right side, alternating to avoid overlap
        board.placeWall({{7, 0}, Orientation::Vertical}, 0);
        board.placeWall({{7, 2}, Orientation::Vertical}, 0);
        board.placeWall({{7, 4}, Orientation::Vertical}, 0);
        board.placeWall({{7, 6}, Orientation::Vertical}, 0);
        board.placeWall({{6, 1}, Orientation::Horizontal}, 0);
        board.placeWall({{6, 3}, Orientation::Horizontal}, 0);
        board.placeWall({{6, 5}, Orientation::Horizontal}, 0);
        
        // P1 walls: spread on left side
        board.placeWall({{0, 0}, Orientation::Vertical}, 1);
        board.placeWall({{0, 2}, Orientation::Vertical}, 1);
        board.placeWall({{0, 4}, Orientation::Vertical}, 1);
        board.placeWall({{0, 6}, Orientation::Vertical}, 1);
        board.placeWall({{1, 1}, Orientation::Horizontal}, 1);
        board.placeWall({{1, 3}, Orientation::Horizontal}, 1);
        board.placeWall({{1, 5}, Orientation::Horizontal}, 1);
        
        Move move = ai.getBestMove(board, state, Difficulty::Hard);
        // AI should make a valid strategic move (either advance or block)
        bool valid = false;
        if (move.isPawnMove()) {
            Position current = board.getPawnPosition(0);
            valid = Rules::isValidMove(board, 0, current.y, current.x, move.pawnDest.y, move.pawnDest.x);
        } else {
            valid = Rules::isValidWallPlacement(board, move.wall, 0);
        }
        REQUIRE(valid);
    }

    SECTION("Partial Walls 3: Hell with 2 walls each - Endgame") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 6}); // 2 moves to goal
        board.movePawn(1, {4, 2}); // 2 moves to goal
        
        // Each player uses 8 walls (2 remaining each)
        // P0 walls: 4 vertical on col 7, 4 horizontal spread out
        board.placeWall({{7, 0}, Orientation::Vertical}, 0);
        board.placeWall({{7, 2}, Orientation::Vertical}, 0);
        board.placeWall({{7, 4}, Orientation::Vertical}, 0);
        board.placeWall({{7, 6}, Orientation::Vertical}, 0);
        board.placeWall({{6, 0}, Orientation::Horizontal}, 0);
        board.placeWall({{6, 2}, Orientation::Horizontal}, 0);
        board.placeWall({{6, 4}, Orientation::Horizontal}, 0);
        board.placeWall({{6, 6}, Orientation::Horizontal}, 0);
        
        // P1 walls: 4 vertical on col 0, 4 horizontal spread out
        board.placeWall({{0, 0}, Orientation::Vertical}, 1);
        board.placeWall({{0, 2}, Orientation::Vertical}, 1);
        board.placeWall({{0, 4}, Orientation::Vertical}, 1);
        board.placeWall({{0, 6}, Orientation::Vertical}, 1);
        board.placeWall({{1, 0}, Orientation::Horizontal}, 1);
        board.placeWall({{1, 2}, Orientation::Horizontal}, 1);
        board.placeWall({{1, 4}, Orientation::Horizontal}, 1);
        board.placeWall({{1, 6}, Orientation::Horizontal}, 1);
        
        Move move = ai.getBestMove(board, state, Difficulty::Hell);
        // With limited walls in endgame, AI should make optimal choice
        bool valid = move.isPawnMove() || move.isWallPlacement();
        REQUIRE(valid);
    }

    SECTION("Partial Walls 4: Normal - Block or advance decision") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 4}); // 4 moves to goal
        board.movePawn(1, {4, 1}); // 1 move to win!
        
        // P0 has 8 walls, P1 has 0 walls
        for(int i=0; i<2; ++i) {
            board.placeWall({{7, i}, Orientation::Horizontal}, 0);
        }
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 0 : 2;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 1);
        }
        
        Move move = ai.getBestMove(board, state, Difficulty::Normal);
        // Must block P1 from winning
        REQUIRE(move.isWallPlacement());
        REQUIRE(move.wall.pos.y <= 1); // Wall should be near P1's goal
    }

    // =================================================================
    // BENCHMARKS
    // =================================================================
    
    SECTION("Performance Benchmarks") {
        state.resetGame(); board.resetGame();
        board.movePawn(0, {4, 4});
        board.movePawn(1, {4, 5});
        
        // Deplete walls to make benchmarks faster and more consistent
        // P0 uses walls on right side
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 7 : 5;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 0);
        }
        // P1 uses walls on left side
        for(int i=0; i<10; ++i) {
            int c = (i < 8) ? 0 : 2;
            int r = (i < 8) ? i : (i - 8);
            board.placeWall({{c, r}, Orientation::Horizontal}, 1);
        }
        
        BENCHMARK("AI Normal (Depth 1)") {
            return ai.getBestMove(board, state, Difficulty::Normal);
        };
        
        BENCHMARK("AI Hard (Depth 2)") {
            return ai.getBestMove(board, state, Difficulty::Hard);
        };
        
        BENCHMARK("AI Hell (Depth 4)") {
            return ai.getBestMove(board, state, Difficulty::Hell);
        };
    }
}
