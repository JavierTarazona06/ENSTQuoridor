#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include "model/Board.hpp"

using namespace Quoridor;

TEST_CASE("Board initialization and positions", "[board]") {
    Board board;

    SECTION("Board size is 9x9") {
        REQUIRE(BOARD_SIZE == 9);
    }

    SECTION("Player 0 starts at (4, 0)") {
        Position p0_start = board.getPawnPosition(0);
        REQUIRE(p0_start.x == 4);
        REQUIRE(p0_start.y == 0);
    }

    SECTION("Player 1 starts at (4, 8)") {
        Position p1_start = board.getPawnPosition(1);
        REQUIRE(p1_start.x == 4);
        REQUIRE(p1_start.y == 8);
    }
}

TEST_CASE("Pawn movement and access", "[board][pawn]") {
    Board board;
    
    SECTION("Move player 0 to valid position") {
        Position p0_new = {4, 1};
        board.movePawn(0, p0_new);
        Position p0_current = board.getPawnPosition(0);
        
        REQUIRE(p0_current == p0_new);
    }
    
    SECTION("Move to out of bounds throws exception") {
        REQUIRE_THROWS_AS(board.movePawn(0, {9, 5}), std::invalid_argument);
    }
    
    SECTION("Invalid player index throws exception on move") {
        REQUIRE_THROWS_AS(board.movePawn(-1, {4, 4}), std::out_of_range);
        REQUIRE_THROWS_AS(board.movePawn(2, {4, 4}), std::out_of_range);
    }

    SECTION("Invalid player index throws exception on get position") {
        REQUIRE_THROWS_AS(board.getPawnPosition(-1), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPawnPosition(2), std::out_of_range);
    }

    SECTION("Invalid player index throws exception on get color") {
        REQUIRE_THROWS_AS(board.getPawnColor(-1), std::out_of_range);
        REQUIRE_THROWS_AS(board.getPawnColor(2), std::out_of_range);
    }
}

TEST_CASE("Wall placement", "[board][wall]") {
    Board board;
    
    SECTION("Place valid horizontal wall") {
        Wall wall1 = {{3, 3}, Orientation::Horizontal};
        board.placeWall(wall1);
        REQUIRE(board.getWalls().size() == 1);
    }

    SECTION("Place valid vertical wall") {
        Wall wall2 = {{0, 0}, Orientation::Vertical};
        board.placeWall(wall2);
        REQUIRE(board.getWalls().size() == 1);
    }
    
    SECTION("Place wall in invalid area throws exception") {
        // Wall placement is 0-7, so 8 is invalid
        REQUIRE_THROWS_AS(
            board.placeWall({{8, 5}, Orientation::Horizontal}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            board.placeWall({{5, 8}, Orientation::Horizontal}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            board.placeWall({{-1, 0}, Orientation::Horizontal}),
            std::invalid_argument
        );
    }
}

TEST_CASE("Board utility functions", "[board]") {
    Board board;

    SECTION("isInBounds validates positions correctly") {
        REQUIRE(Board::isInBounds(0, 0) == true);
        REQUIRE(Board::isInBounds(8, 8) == true);
        REQUIRE(Board::isInBounds(-1, 0) == false);
        REQUIRE(Board::isInBounds(0, 9) == false);
    }

    SECTION("setPawnPosition works correctly") {
        board.setPawnPosition(0, 2, 3); // Player 0 to row 2, col 3
        Position p0 = board.getPawnPosition(0);
        REQUIRE(p0.x == 3);
        REQUIRE(p0.y == 2);
    }

    SECTION("setPawnPosition out of bounds throws exception") {
        REQUIRE_THROWS_AS(
            board.setPawnPosition(0, 9, 9),
            std::invalid_argument
        );
    }
}
