#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "model/Board.hpp"
#include "model/Pathfinder.hpp"

using namespace Quoridor;

TEST_CASE("Pathfinder Basic", "[pathfinder]") {
    Board board;
    
    SECTION("Empty board has path for both players") {
        REQUIRE(Pathfinder::hasPathToGoal(board, 0) == true);
        REQUIRE(Pathfinder::hasPathToGoal(board, 1) == true);
    }
}

TEST_CASE("Pathfinder with Walls", "[pathfinder]") {
    Board board;

    SECTION("Path blocked by single wall is not possible (walls are small)") {
        // Walls are 2 units wide, but board is 9 wide.
        // A single wall cannot block the whole path.
        Wall w = {{4, 0}, Orientation::Horizontal}; // Blocks (4,0)-(4,1) and (5,0)-(5,1)??
        // Wait, horizontal wall at (x,y) blocks vertical movement across row y and y+1 at cols x and x+1.
        // Actually wall coordinates are top-left of the 2x2 area centered on the intersection.
        // Horizontal wall at (x,y) is between row y and y+1, spanning col x and x+1.
        
        // Let's rely on hasPathToGoal(..., wall)
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == true);
    }

    SECTION("Path completely blocked by line of walls") {
        // Construct a line of walls blocking player 0 from reaching row 8
        // Player 0 starts at (4,0).
        // Let's block row 7 to 8 transition completely.
        // Walls at y=7. x=0, 2, 4, 6, 8?
        // Wall at (0,7) H spans 0,1.
        // Wall at (2,7) H spans 2,3.
        // Wall at (4,7) H spans 4,5.
        // Wall at (6,7) H spans 6,7.
        // Wall at (8,7) is invalid (x max is 7).
        // We need to block col 8.
        // Wall at (7,7) H spans 7,8.
        
        // Let's place walls for x=0, 2, 4, 6 using placeWall
        // Note: Rules enforce wall count, but Board::placeWall might not?
        // Board::placeWall just adds it to the vector.
        
        board.placeWall({{0, 7}, Orientation::Horizontal}, 1);
        board.placeWall({{2, 7}, Orientation::Horizontal}, 1);
        board.placeWall({{4, 7}, Orientation::Horizontal}, 1);
        board.placeWall({{6, 7}, Orientation::Horizontal}, 1);
        
        // Now use hasPathToGoal with a hypothetical wall that seals the deal
        Wall finalWall = {{7, 7}, Orientation::Horizontal}; // Spans 7,8
        
        // This should return false because path is blocked
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, finalWall) == false);
        
        // Without the final wall, path should exist
        REQUIRE(Pathfinder::hasPathToGoal(board, 0) == true);
    }

    SECTION("Maze - complex path") {
        // Create a U-shape cup that forces the player to walk around
        // Player 0 at (4,0).
        
        // We want to force a detour.
        // Block Right side: Vertical wall at (4,0) -> blocks (4,0)-(5,0)
        board.placeWall({{4, 0}, Orientation::Vertical}, 1);
        
        // Block Down side with hypothetical wall: Horizontal wall at (4,0) -> blocks (4,0)-(4,1)
        
        // Player 0 must go Left to (3,0), then Down.
        // Ensure Left is open (no wall at 3,0).
        
        Wall w = {{4, 0}, Orientation::Horizontal};
        // Verify path still exists (detour left)
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == true);
    }
    
    SECTION("No path (Encased)") {
        // Player 0 at (4,0).
        // Encase (4,0) completely.
        board.placeWall({{3, 0}, Orientation::Vertical}, 1); // Block Left
        board.placeWall({{4, 0}, Orientation::Vertical}, 1); // Block Right
        
        Wall w = {{4, 0}, Orientation::Horizontal}; // Block Down
        
        // Path should be blocked
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == false);
    }

    SECTION("Multiple paths available, one blocked") {
        // P0 at (4,0). Goal Row 8.
        // Paths: Go Left then Down, or Go Right then Down.
        // Block Left path.
        board.placeWall({{3, 0}, Orientation::Vertical}, 1); // Blocks (3,0)-(4,0) and (3,1)-(4,1)
        // Now Left is blocked.
        // Block Down path.
        board.placeWall({{4, 0}, Orientation::Horizontal}, 1); // Blocks Down.
        
        // But Right path is open. (no wall at {{4, 0}, Vertical})
        
        // Hypothetical wall blocking far Left?
        Wall w = {{0, 0}, Orientation::Horizontal};
        
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == true);
    }

    SECTION("Wall blocks ONE player but not other") {
        // P0 at (4,0). Goal Row 8.
        // P1 at (4,8). Goal Row 0.
        // Encase P0.
        board.placeWall({{3, 0}, Orientation::Vertical}, 1);
        board.placeWall({{4, 0}, Orientation::Vertical}, 1);
        
        // Hypothetical wall encasing P0 fully.
        Wall w = {{4, 0}, Orientation::Horizontal};
        
        // P0 blocked.
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == false);
        
        // P1 not blocked (can reach row 0 via many paths).
        REQUIRE(Pathfinder::hasPathToGoal(board, 1, w) == true);
    }
    
    BENCHMARK("Pathfinding on empty board") {
        return Pathfinder::hasPathToGoal(board, 0);
    };
    
    // Setup maze board for benchmark
    // Use fewer walls to stay within limit (10 per player)
    // Or alternate players.
    // 16 walls needed. P0 takes 8, P1 takes 8.
    Board mazeBoard;
    for(int r=0; r<8; r+=2) {
        int p = (r/2) % 2; // Alternate 0 and 1
        mazeBoard.placeWall({{0, r}, Orientation::Horizontal}, p); 
        mazeBoard.placeWall({{2, r}, Orientation::Horizontal}, p);
        mazeBoard.placeWall({{4, r}, Orientation::Horizontal}, 1-p);
        mazeBoard.placeWall({{6, r}, Orientation::Horizontal}, 1-p);
    }

    BENCHMARK("Pathfinding on maze board") {
        return Pathfinder::hasPathToGoal(mazeBoard, 0);
    };
}
