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

    SECTION("Empty board shortest distance is correct") {
        // P0 at (4,0) to Row 8. Distance is 8.
        REQUIRE(Pathfinder::getShortestPathDistance(board, 0) == 8);
        
        // P1 at (4,8) to Row 0. Distance is 8.
        REQUIRE(Pathfinder::getShortestPathDistance(board, 1) == 8);
    }
}

TEST_CASE("Pathfinder with Walls", "[pathfinder]") {
    Board board;

    SECTION("Path blocked by single wall is not possible (walls are small)") {
        Wall w = {{4, 0}, Orientation::Horizontal}; 
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == true);
    }

    SECTION("Path completely blocked by line of walls") {
        board.placeWall({{0, 7}, Orientation::Horizontal}, 1);
        board.placeWall({{2, 7}, Orientation::Horizontal}, 1);
        board.placeWall({{4, 7}, Orientation::Horizontal}, 1);
        board.placeWall({{6, 7}, Orientation::Horizontal}, 1);
        
        Wall finalWall = {{7, 7}, Orientation::Horizontal}; // Spans 7,8
        
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, finalWall) == false);
        REQUIRE(Pathfinder::hasPathToGoal(board, 0) == true);
    }

    SECTION("Maze - complex path") {
        // Block Right side at (4,0) -> blocks (4,0)-(5,0)
        board.placeWall({{4, 0}, Orientation::Vertical}, 1);
        
        // Hypothetical wall blocking Down at (4,0) -> blocks (4,0)-(4,1)
        // Note: Using {{4, 0}, Horizontal} would cross the Vertical wall at {{4, 0}}, which is invalid.
        // We use {{3, 0}, Horizontal} which spans cols 3 and 4, effectively blocking (3,0)-(3,1) AND (4,0)-(4,1).
        // This avoids the crossing violation.
        Wall w = {{3, 0}, Orientation::Horizontal};
        
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == true);
        
        // Distance check: P0 at (4,0). Needs to go Left(3,0), Down...
        // Original dist 8.
        // With Vertical wall at (4,0), moving right is blocked.
        // With Horizontal wall at (3,0), moving down from (4,0) to (4,1) is blocked.
        // So P0 must go Left to (3,0).
        // From (3,0), moving down to (3,1) is ALSO blocked by the same horizontal wall at (3,0).
        // So P0 must go to (2,0) or wrap around.
        // Let's see: (4,0)->(3,0) [1 step]
        // At (3,0): Down blocked. Right is (4,0) (back). Left is (2,0).
        // So (3,0)->(2,0) [1 step].
        // At (2,0): Can go down to (2,1).
        // Then (2,1)->...->(2,8) is 7 steps.
        // Total: 1 (left) + 1 (left) + 1 (down) + 7 (down) = 10 steps.
        // Or similar. It definitely increases distance.
        
        board.placeWall(w, 1);
        // Distance should be at least increased.
        // Path: (4,0)->(3,0)->...->(3,8) = 1 (left) + 8 (down) = 9.
        // Or (4,0)->(5,0) is blocked by Vertical wall.
        // So must go left.
        
        REQUIRE(Pathfinder::getShortestPathDistance(board, 0) >= 9);
    }
    
    SECTION("No path (Encased)") {
        board.placeWall({{3, 0}, Orientation::Vertical}, 1); // Block Left
        board.placeWall({{4, 0}, Orientation::Vertical}, 1); // Block Right
        Wall w = {{4, 0}, Orientation::Horizontal}; // Block Down
        
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == false);
    }

    SECTION("Multiple paths available, one blocked") {
        board.placeWall({{3, 0}, Orientation::Vertical}, 1); 
        board.placeWall({{4, 0}, Orientation::Horizontal}, 1); 
        Wall w = {{0, 0}, Orientation::Horizontal};
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == true);
    }

    SECTION("Wall blocks ONE player but not other") {
        board.placeWall({{3, 0}, Orientation::Vertical}, 1);
        board.placeWall({{4, 0}, Orientation::Vertical}, 1);
        Wall w = {{4, 0}, Orientation::Horizontal};
        
        REQUIRE(Pathfinder::hasPathToGoal(board, 0, w) == false);
        REQUIRE(Pathfinder::hasPathToGoal(board, 1, w) == true);
    }
    
    BENCHMARK("A* Pathfinding on empty board") {
        return Pathfinder::getShortestPathDistance(board, 0);
    };
    
    Board mazeBoard;
    for(int r=0; r<8; r+=2) {
        int p = (r/2) % 2; 
        mazeBoard.placeWall({{0, r}, Orientation::Horizontal}, p); 
        mazeBoard.placeWall({{2, r}, Orientation::Horizontal}, p);
        mazeBoard.placeWall({{4, r}, Orientation::Horizontal}, 1-p);
        mazeBoard.placeWall({{6, r}, Orientation::Horizontal}, 1-p);
    }

    BENCHMARK("A* Pathfinding on maze board") {
        return Pathfinder::getShortestPathDistance(mazeBoard, 0);
    };
}
