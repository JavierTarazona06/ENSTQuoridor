#include <catch2/catch_test_macros.hpp>
#include "model/Rules.hpp"
#include "model/Board.hpp"

using namespace Quoridor;

TEST_CASE("Wall placement validity (Basic)", "[wall][validation]") {
    Board board;

    SECTION("Valid Horizontal Placement") {
        Wall w = {{0, 0}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w, 0));
    }

    SECTION("Valid Vertical Placement") {
        Wall w = {{0, 0}, Orientation::Vertical};
        REQUIRE(Rules::isValidWallPlacement(board, w, 1));
    }

    SECTION("Valid Placement at limits (7,7)") {
        Wall w = {{7, 7}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w, 0));
    }
}

TEST_CASE("Wall placement bounds", "[wall][validation]") {
    Board board;

    SECTION("X too small (-1)") {
        Wall w = {{-1, 0}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 0));
    }

    SECTION("X too large (8)") {
        Wall w = {{8, 0}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 0));
    }

    SECTION("Y too small (-1)") {
        Wall w = {{0, -1}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 0));
    }

    SECTION("Y too large (8)") {
        Wall w = {{0, 8}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 0));
    }
}

TEST_CASE("Wall overlap validation", "[wall][collision]") {
    Board board;
    // Place a reference wall at (3,3) Horizontal
    Wall refWall = {{3, 3}, Orientation::Horizontal};
    board.placeWall(refWall, 0);

    SECTION("Exact overlap (Horizontal on Horizontal)") {
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, refWall, 1));
    }

    SECTION("Crossing overlap (Vertical on Horizontal)") {
        Wall vWall = {{3, 3}, Orientation::Vertical};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, vWall, 1));
    }

    SECTION("Partial overlap Horizontal (Left)") {
        Wall w = {{2, 3}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 1));
    }

    SECTION("Partial overlap Horizontal (Right)") {
        Wall w = {{4, 3}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 1));
    }

    SECTION("No overlap Horizontal (Far Left)") {
        Wall w = {{1, 3}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w, 1));
    }
}

TEST_CASE("Wall overlap validation (Vertical)", "[wall][collision]") {
    Board board;
    // Place a reference wall at (3,3) Vertical
    Wall refWall = {{3, 3}, Orientation::Vertical};
    board.placeWall(refWall, 0);

    SECTION("Exact overlap (Vertical on Vertical)") {
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, refWall, 1));
    }

    SECTION("Crossing overlap (Horizontal on Vertical)") {
        Wall hWall = {{3, 3}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, hWall, 1));
    }

    SECTION("Partial overlap Vertical (Up)") {
        Wall w = {{3, 2}, Orientation::Vertical};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 1));
    }

    SECTION("Partial overlap Vertical (Down)") {
        Wall w = {{3, 4}, Orientation::Vertical};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 1));
    }
}

TEST_CASE("Wall count validation", "[wall][count]") {
    Board board;
    
    // Simulate player 0 using all walls (10 walls)
    // Place walls avoiding overlaps (length 2)
    // Row 0: (0,0), (2,0), (4,0), (6,0) -> 4 walls
    board.placeWall({{0, 0}, Orientation::Horizontal}, 0);
    board.placeWall({{2, 0}, Orientation::Horizontal}, 0);
    board.placeWall({{4, 0}, Orientation::Horizontal}, 0);
    board.placeWall({{6, 0}, Orientation::Horizontal}, 0);
    
    // Row 2: (0,2), (2,2), (4,2), (6,2) -> 4 walls
    board.placeWall({{0, 2}, Orientation::Horizontal}, 0);
    board.placeWall({{2, 2}, Orientation::Horizontal}, 0);
    board.placeWall({{4, 2}, Orientation::Horizontal}, 0);
    board.placeWall({{6, 2}, Orientation::Horizontal}, 0);

    // Row 4: (0,4), (2,4) -> 2 walls
    board.placeWall({{0, 4}, Orientation::Horizontal}, 0);
    board.placeWall({{2, 4}, Orientation::Horizontal}, 0);
    
    SECTION("No walls remaining") {
        REQUIRE(board.getWallsRemaining(0) == 0);
        Wall w = {{7, 7}, Orientation::Horizontal};
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, w, 0));
    }

    SECTION("Other player still has walls") {
        REQUIRE(board.getWallsRemaining(1) == 10);
        Wall w = {{7, 7}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w, 1));
    }
}

TEST_CASE("Path blocking validation (Anti-blocking)", "[wall][pathfinding]") {
    Board board;

    SECTION("Simple cage for Player 0") {
        // Player 0 is at (4,0). Goal is Row 8.
        // Block (4,0) with walls.
        // (4,0) has neighbors (3,0), (5,0), (4,1).
        
        // Vertical at (3,0) blocks LEFT (between 3,0 and 4,0)
        board.placeWall({{3, 0}, Orientation::Vertical}, 1);
        
        // Vertical at (4,0) blocks RIGHT (between 4,0 and 5,0)
        board.placeWall({{4, 0}, Orientation::Vertical}, 1);
        
        // Horizontal at (4,0) blocks DOWN (between 4,0 and 4,1)
        Wall sealingWall = {{4, 0}, Orientation::Horizontal};
        
        // This wall should be invalid as it traps P0
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, sealingWall, 1));
    }

    SECTION("Simple cage for Player 1") {
        // Player 1 is at (4,8). Goal is Row 0.
        // Block (4,8) with walls.
        
        // Vertical at (3,7) -> blocks LEFT of (4,7) and (4,8)?
        // Vertical wall at (x,y) blocks col x|x+1 for rows y, y+1.
        // V(3,7) blocks col 3|4 for rows 7,8. Correct.
        board.placeWall({{3, 7}, Orientation::Vertical}, 0);
        
        // Vertical at (4,7) -> blocks col 4|5 for rows 7,8. Correct.
        board.placeWall({{4, 7}, Orientation::Vertical}, 0);
        
        // Horizontal at (4,7) -> blocks row 7|8 for cols 4,5. Correct.
        Wall sealingWall = {{4, 7}, Orientation::Horizontal};
        
        REQUIRE_FALSE(Rules::isValidWallPlacement(board, sealingWall, 0));
    }

    SECTION("Labyrinth path exists") {
        // Construct a wall that makes the path long but possible.
        // P0 at (4,0). Wall at (3,0) H, (5,0) H?
        // Just verify a wall that doesn't block is valid.
        
        Wall w = {{4, 1}, Orientation::Horizontal};
        // This makes P0 go around, but not blocked.
        REQUIRE(Rules::isValidWallPlacement(board, w, 1));
    }
}
