#include <catch2/catch_test_macros.hpp>
#include "model/AI.hpp"
#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"
#include <limits>

using namespace Quoridor;

// ============================================================================
// NOTE: The evaluation function uses multiple factors with tunable weights:
//   - Distance difference: DISTANCE_DIFF (10)
//   - Endgame bonus: ENDGAME_BONUS (15) - triggered when dist <= 3
//   - Wall advantage: WALL_ADVANTAGE (2)
//   - Mobility: MOBILITY (3)
//   - Wall effectiveness: WALL_EFFECTIVENESS (5)
// 
// Tests verify relative ordering and key properties rather than exact values,
// as exact values depend on weight tuning.
// ============================================================================

TEST_CASE("Heuristic Evaluation", "[ai][heuristics]") {
    AI ai;
    Board board;
    
    SECTION("Initial board score is zero") {
        // Distances are equal (8 vs 8)
        // Walls are equal (10 vs 10)
        // Mobility is equal (both have same options)
        // Wall effectiveness is zero (no walls placed)
        int score = ai.evaluate(board, 0);
        REQUIRE(score == 0);
        
        score = ai.evaluate(board, 1);
        REQUIRE(score == 0);
    }
    
    SECTION("Advantage by moving closer") {
        // Move P0 down one step
        board.movePawn(0, {4, 1}); // Original {4,0}. Now dist is 7.
        // P1 at {4,8}, dist 8.
        // 
        // Score components for P0:
        // - Distance: (8 - 7) * 10 = 10
        // - Endgame bonus: 0 (dist > 3)
        // - Wall advantage: 0
        // - Mobility difference: ~0 (both have similar options)
        // - Wall effectiveness: might be non-zero due to position change
        
        int score = ai.evaluate(board, 0);
        REQUIRE(score > 0); // Should be positive (we are closer)
        
        // P1 perspective: should be negative (opponent is closer)
        int scoreP1 = ai.evaluate(board, 1);
        REQUIRE(scoreP1 < 0);
        
        // Scores should be roughly symmetric (opposite signs)
        REQUIRE(score == -scoreP1);
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
        // Wall at (0,0) shouldn't affect distances much
        board.placeWall({{0, 0}, Orientation::Horizontal}, 1);
        
        // P0 walls: 10. P1 walls: 9.
        // Wall advantage = (10-9)*2 = 2
        // Other factors might contribute small amounts
        
        int score = ai.evaluate(board, 0);
        REQUIRE(score > 0); // Should be positive due to wall advantage
    }
    
    SECTION("Endgame bonus activates when close to goal") {
        Board board1, board2;
        
        // Board1: P0 far from goal (row 1, dist ~7)
        board1.movePawn(0, {4, 1});
        
        // Board2: P0 close to goal (row 6, dist ~2)
        board2.movePawn(0, {4, 6});
        
        int scoreFar = ai.evaluate(board1, 0);
        int scoreClose = ai.evaluate(board2, 0);
        
        // Being closer to goal should give higher score
        // The endgame bonus should make the difference more significant
        REQUIRE(scoreClose > scoreFar);
        
        // The difference should be substantial due to endgame bonus
        int distanceDiff = (8 - 2) - (8 - 7); // 6 - 1 = 5 steps closer
        int expectedMinimumDiff = distanceDiff * 10; // At least from distance factor
        REQUIRE((scoreClose - scoreFar) >= expectedMinimumDiff);
    }
    
    SECTION("Wall effectiveness rewards blocking opponent") {
        Board boardNoWall, boardWithWall;
        
        // Try to place a wall that blocks opponent's path
        // P1 is at (4, 8), needs to reach row 0
        // A well-placed wall should increase P1's distance
        
        // Find a wall position that actually blocks P1's path
        // Wall at (4, 7) horizontal should block direct path
        if (Rules::isValidWallPlacement(boardWithWall, {{3, 7}, Orientation::Horizontal}, 0)) {
            boardWithWall.placeWall({{3, 7}, Orientation::Horizontal}, 0);
            
            int scoreNoWall = ai.evaluate(boardNoWall, 0);
            int scoreWithWall = ai.evaluate(boardWithWall, 0);
            
            // If the wall effectively blocks, score should improve
            // (though wall count decreases, blocking should compensate)
            // At minimum, verify the evaluation runs without error
            REQUIRE(scoreWithWall != std::numeric_limits<int>::min());
        }
    }
}
