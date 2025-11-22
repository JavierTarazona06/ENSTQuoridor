#include <catch2/catch_test_macros.hpp>
#include "model/State.hpp"

using namespace Quoridor;

TEST_CASE("State initialization", "[state]") {
    Board board;
    State state(board);
    
    SECTION("Initial player is 0") {
        REQUIRE(state.getCurrentPlayer() == 0);
    }
    
    SECTION("Initial status is Playing") {
        REQUIRE(state.getGameStatus() == GameStatus::Playing);
    }
}

TEST_CASE("Player switching", "[state]") {
    Board board;
    State state(board);
    
    SECTION("Switch from player 0 to 1") {
        state.switchPlayer();
        REQUIRE(state.getCurrentPlayer() == 1);
    }
    
    SECTION("Switch back from player 1 to 0") {
        state.switchPlayer();
        state.switchPlayer();
        REQUIRE(state.getCurrentPlayer() == 0);
    }
}

TEST_CASE("Board integration", "[state][board]") {
    Board board;
    State state(board);
    
    SECTION("Board is accessible and initialized") {
        Position p0 = state.getBoard().getPawnPosition(0);
        REQUIRE(p0.x == 4);
        REQUIRE(p0.y == 0);
    }
    
    SECTION("Board can be modified through State") {
        state.getBoard().setPawnPosition(0, 1, 1);
        Position p0_new = state.getBoard().getPawnPosition(0);
        REQUIRE(p0_new.x == 1);
        REQUIRE(p0_new.y == 1);
    }
}
