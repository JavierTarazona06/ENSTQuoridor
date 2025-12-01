#include <catch2/catch_test_macros.hpp>
#include "model/State.hpp"

using namespace Quoridor;

TEST_CASE("State initialization", "[state]") {
    State state;
    
    SECTION("Initial player is 0") {
        REQUIRE(state.getCurrentPlayer() == 0);
    }
    
    SECTION("Initial status is Playing") {
        REQUIRE(state.getGameStatus() == GameStatus::Playing);
    }
}

TEST_CASE("Player switching", "[state]") {
    State state;
    
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

TEST_CASE("State reset", "[state]") {
    State state;
    
    SECTION("Reset restores initial state") {
        state.switchPlayer();
        state.setGameStatus(GameStatus::Player1Won);
        state.setSelectedPawn(Position{3, 3});
        
        state.resetGame();
        
        REQUIRE(state.getCurrentPlayer() == 0);
        REQUIRE(state.getGameStatus() == GameStatus::Playing);
        REQUIRE(!state.getSelectedPawn().has_value());
    }
}
