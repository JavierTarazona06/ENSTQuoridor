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
        board.placeWall(hWall);
        
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 1, 4) == false);
    }
    
    SECTION("Vertical wall blocks movement left") {
        // Place vertical wall at (3, 0)
        Wall vWall = {{3, 0}, Orientation::Vertical};
        board.placeWall(vWall);
        
        REQUIRE(Rules::isValidMove(board, 0, 0, 4, 0, 3) == false);
    }
    
    SECTION("Movement to right is unblocked") {
        // Place walls that don't block right movement
        Wall hWall = {{4, 0}, Orientation::Horizontal};
        Wall vWall = {{3, 0}, Orientation::Vertical};
        board.placeWall(hWall);
        board.placeWall(vWall);
        
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
