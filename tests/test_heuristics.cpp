#include <catch2/catch_test_macros.hpp>
#include "model/AI.hpp"
#include "model/Board.hpp"
#include "model/State.hpp"
#include <limits>

using namespace Quoridor;

TEST_CASE("Heuristic Evaluation", "[ai][heuristics]") {
    AI ai;
    Board board;
    
    SECTION("Initial board score is zero") {
        // Distances are equal (8 vs 8)
        // Walls are equal (10 vs 10)
        int score = ai.evaluate(board, 0);
        REQUIRE(score == 0);
        
        score = ai.evaluate(board, 1);
        REQUIRE(score == 0);
    }
    
    SECTION("Advantage by moving closer") {
        // Move P0 down one step
        board.movePawn(0, {4, 1}); // Original {4,0}. Now dist is 7.
        // P1 at {4,8}, dist 8.
        // Score = (8 - 7)*10 = 10.
        
        int score = ai.evaluate(board, 0);
        REQUIRE(score == 10);
        
        // P1 perspective: Opponent(7) - Me(8) = -1. Score -10.
        int scoreP1 = ai.evaluate(board, 1);
        REQUIRE(scoreP1 == -10);
    }
    
    SECTION("Winning state") {
        // P0 at row 8
        board.movePawn(0, {4, 8});
        int score = ai.evaluate(board, 0);
        REQUIRE(score == std::numeric_limits<int>::max());
    }
    
    SECTION("Losing state") {
        // P1 at row 0
        board.movePawn(1, {4, 0});
        int score = ai.evaluate(board, 0);
        REQUIRE(score == std::numeric_limits<int>::min());
    }
    
    SECTION("Wall bonus") {
        // Place a wall for Player 1 (uses 1 wall)
        // Place wall far away to not affect distances (e.g. corner)
        // Horizontal wall at (0,0) might block path slightly if P0 is there?
        // P0 at (4,0). Wall at (0,0) is fine.
        board.placeWall({{0, 0}, Orientation::Horizontal}, 1);
        
        // P0 walls: 10. P1 walls: 9.
        // Distances: 8 vs 8 (assuming path dist didn't change).
        // Score = (8-8)*10 + (10-9)*1 = 1.
        
        int score = ai.evaluate(board, 0);
        REQUIRE(score == 1);
    }
}
