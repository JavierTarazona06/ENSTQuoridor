#include <catch2/catch_test_macros.hpp>
#include "model/Rules.hpp"
#include "model/Board.hpp"

using namespace Quoridor;

TEST_CASE("Basic movement validation", "[rules][movement]") {
    Board board;
    
    SECTION("Valid move down") {
        // Player 0 starts at (4, 0)
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 1, 4) == true);
    }

    SECTION("Valid move up") {
        // Move Player 0 to (4, 1) first
        board.setPawnPosition(0, 1, 4);
        REQUIRE(Rules::isValidMove(board, 0, 1, 4, 0, 4) == true);
    }
    
    SECTION("Valid move left") {
        // Move Player 0 to (4, 1) first
        board.setPawnPosition(0, 1, 4);
        REQUIRE(Rules::isValidMove(board, 0, 1, 4, 1, 3) == true);
    }

    SECTION("Valid move right") {
        // Move Player 0 to (4, 1) first
        board.setPawnPosition(0, 1, 4);
        REQUIRE(Rules::isValidMove(board, 0, 1, 4, 1, 5) == true);
    }
    
    SECTION("Invalid diagonal move") {
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 1, 5) == false);
    }
    
    SECTION("Invalid jump (2 steps)") {
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 2, 4) == false);
    }
    
    SECTION("Invalid out of bounds") {
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, -1, 4) == false);
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 0, 9) == false);
    }

    SECTION("Invalid stationary move") {
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 0, 4) == false);
    }

    SECTION("Corner movement validation (Top-Left)") {
        board.setPawnPosition(0, 0, 0);
        // Valid moves
        REQUIRE(Rules::isValidMove(board, 0, 0, 0, 0, 1) == true); // Right
        REQUIRE(Rules::isValidMove(board, 0, 0, 0, 1, 0) == true); // Down
        // Invalid moves (out of bounds)
        REQUIRE(Rules::isValidMove(board, 0, 0, 0, 0, -1) == false); // Left
        REQUIRE(Rules::isValidMove(board, 0, 0, 0, -1, 0) == false); // Up
    }

    SECTION("Corner movement validation (Bottom-Right)") {
        board.setPawnPosition(0, 8, 8);
        // Valid moves
        REQUIRE(Rules::isValidMove(board, 0, 8, 8, 8, 7) == true); // Left
        REQUIRE(Rules::isValidMove(board, 0, 8, 8, 7, 8) == true); // Up
        // Invalid moves (out of bounds)
        REQUIRE(Rules::isValidMove(board, 0, 8, 8, 8, 9) == false); // Right
        REQUIRE(Rules::isValidMove(board, 0, 8, 8, 9, 8) == false); // Down
    }
}

TEST_CASE("Collision validation", "[rules][collision]") {
    Board board;
    
    SECTION("Cannot move to occupied cell") {
        // Place P1 adjacent to P0
        board.setPawnPosition(1, 1, 4); // row 1, col 4
        
        // P0 tries to move to (4, 1) which is occupied by P1
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 1, 4) == false);
    }
}

TEST_CASE("Wall blocking", "[rules][walls]") {
    Board board;
    
    SECTION("Horizontal wall blocks movement down") {
        // Place horizontal wall at (4, 0)
        Wall hWall = {{4, 0}, Orientation::Horizontal};
        board.placeWall(hWall, 0);
        
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 1, 4) == false);
    }
    
    SECTION("Vertical wall blocks movement left") {
        // Place vertical wall at (3, 0)
        Wall vWall = {{3, 0}, Orientation::Vertical};
        board.placeWall(vWall, 0);
        
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 0, 3) == false);
    }
    
    SECTION("Movement to right is unblocked") {
        // Place walls that don't block right movement
        Wall hWall = {{4, 0}, Orientation::Horizontal};
        Wall vWall = {{3, 0}, Orientation::Vertical};
        board.placeWall(hWall, 0);
        board.placeWall(vWall, 1);
        
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 0, 5) == true);
    }
}

TEST_CASE("Victory condition", "[rules][victory]") {
    Board board;
    
    SECTION("Player 0 at start position has not won") {
        REQUIRE(Rules::checkVictory(board, 0) == false);
    }
    
    SECTION("Player 0 at row 8 has won") {
        board.setPawnPosition(0, 8, 4);
        REQUIRE(Rules::checkVictory(board, 0) == true);
    }

    SECTION("Player 0 at row 8, different column has won") {
        board.setPawnPosition(0, 8, 0);
        REQUIRE(Rules::checkVictory(board, 0) == true);
    }
    
    SECTION("Player 0 at row 7 has not won") {
        board.setPawnPosition(0, 7, 4);
        REQUIRE(Rules::checkVictory(board, 0) == false);
    }

    SECTION("Player 1 at start position has not won") {
        REQUIRE(Rules::checkVictory(board, 1) == false);
    }

    SECTION("Player 1 at row 0 has won") {
        board.setPawnPosition(1, 0, 4);
        REQUIRE(Rules::checkVictory(board, 1) == true);
    }

    SECTION("Player 1 at row 1 has not won") {
        board.setPawnPosition(1, 1, 4);
        REQUIRE(Rules::checkVictory(board, 1) == false);
    }
}

TEST_CASE("Wall placement validation", "[rules][walls]") {
    Board board;
    
    SECTION("Valid wall placement") {
        Wall w = {{0, 0}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w, 0) == true);
    }

    SECTION("Invalid out of bounds") {
        // Wall grid is 0-7
        Wall w1 = {{8, 0}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w1, 0) == false);
        
        Wall w2 = {{0, 8}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w2, 0) == false);
        
        Wall w3 = {{-1, 0}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w3, 0) == false);
    }

    SECTION("Invalid overlap (exact)") {
        Wall w = {{3, 3}, Orientation::Horizontal};
        board.placeWall(w, 0);
        
        REQUIRE(Rules::isValidWallPlacement(board, w, 1) == false);
    }

    SECTION("Invalid overlap (crossing)") {
        Wall w1 = {{3, 3}, Orientation::Horizontal};
        board.placeWall(w1, 0);
        
        Wall w2 = {{3, 3}, Orientation::Vertical};
        REQUIRE(Rules::isValidWallPlacement(board, w2, 1) == false);
    }

    SECTION("Invalid overlap (partial horizontal)") {
        Wall w1 = {{3, 3}, Orientation::Horizontal};
        board.placeWall(w1, 0);
        
        // Wall at (2,3) overlaps (3,3)
        Wall w2 = {{2, 3}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w2, 1) == false);
        
        // Wall at (4,3) overlaps (3,3)
        Wall w3 = {{4, 3}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, w3, 1) == false);
    }

    SECTION("Invalid overlap (partial vertical)") {
        Wall w1 = {{3, 3}, Orientation::Vertical};
        board.placeWall(w1, 0);
        
        // Wall at (3,2) overlaps (3,3)
        Wall w2 = {{3, 2}, Orientation::Vertical};
        REQUIRE(Rules::isValidWallPlacement(board, w2, 1) == false);
        
        // Wall at (3,4) overlaps (3,3)
        Wall w3 = {{3, 4}, Orientation::Vertical};
        REQUIRE(Rules::isValidWallPlacement(board, w3, 1) == false);
    }
    
    SECTION("Wall count limit") {
        // Use up all walls for player 0
        // We can't easily force wall count to 0 without placing 10 walls or modifying board internals.
        // Board::placeWall decrements count.
        // Let's place 10 walls.
        for(int i=0; i<10; ++i) {
             // Place walls in a non-blocking pattern
             // e.g. horizontal walls at different rows far left
             // (0,0), (0,2), (0,4)... might block movement if not careful.
             // Just place vertical walls at top row: (0,0), (2,0), (4,0), (6,0) -> 4 walls
             // Then second row...
             // Let's just place walls carefully.
             // (0,0) H, (2,0) H, (4,0) H, (6,0) H -> 4 walls
             // (0,2) H ...
             // Actually, simplest is just placing walls far away from center column where pawns are.
        }
        
        // Easier: check logic by relying on Board state.
        // Since we can't easily modify private wall count, we have to simulate usage.
        // But placing 10 walls in a test is tedious.
        // However, the requirement is to validate wall count.
        // I will trust the logic `board.getWallsRemaining(playerIndex) <= 0` and minimal test here.
        // Or I can use a loop.
        
        int x = 0;
        int y = 0;
        for (int i = 0; i < 10; ++i) {
            Wall w = {{x, y}, Orientation::Horizontal};
            if (Rules::isValidWallPlacement(board, w, 0)) {
                board.placeWall(w, 0);
            }
            y += 2; 
            if (y >= 8) { y = 0; x += 2; }
        }
        
        REQUIRE(board.getWallsRemaining(0) == 0);
        Wall wExtra = {{7, 7}, Orientation::Horizontal};
        REQUIRE(Rules::isValidWallPlacement(board, wExtra, 0) == false);
    }

    SECTION("Path blocking validation") {
        // Block Player 0 (at 4,0) from reaching row 8
        // Build a cage around (4,0).
        // Vertical (3,0), Vertical (4,0) - wait, V wall at (3,0) blocks col 3|4. V wall at (4,0) blocks col 4|5.
        // Horizontal (4,0) blocks row 0|1.
        
        // Place V wall at (3,0) -> blocks left side of (4,0)
        board.placeWall({{3, 0}, Orientation::Vertical}, 1);
        
        // Place V wall at (4,0) -> blocks right side of (4,0)
        board.placeWall({{4, 0}, Orientation::Vertical}, 1);
        
        // Now try to place H wall at (4,0) -> blocks bottom of (4,0)
        // This should fully enclose (4,0) because top is board edge.
        Wall sealingWall = {{4, 0}, Orientation::Horizontal};
        
        REQUIRE(Rules::isValidWallPlacement(board, sealingWall, 1) == false);
    }
}
