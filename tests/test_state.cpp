#include <iostream>
#include "model/State.hpp"

using namespace Quoridor;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL] Test line " << __LINE__ << ": " << message << std::endl; \
            return 1; \
        } \
        std::cout << "[PASS] " << message << std::endl; \
    } while(0)

int test_initialization() {
    std::cout << "\n--- TEST: State Initialization ---" << std::endl;
    State state;
    
    // Check initial player is 0
    TEST_ASSERT(state.getCurrentPlayer() == 0, "Initial player is 0");
    
    // Check initial status is Playing
    TEST_ASSERT(state.getGameStatus() == GameStatus::Playing, "Initial status is Playing");
    
    return 0;
}

int test_player_switching() {
    std::cout << "\n--- TEST: Player Switching ---" << std::endl;
    State state;
    
    state.switchPlayer();
    TEST_ASSERT(state.getCurrentPlayer() == 1, "Player switched to 1");
    
    state.switchPlayer();
    TEST_ASSERT(state.getCurrentPlayer() == 0, "Player switched back to 0");
    
    return 0;
}

int test_board_integration() {
    std::cout << "\n--- TEST: Board Integration ---" << std::endl;
    State state;
    
    // Verify we can access the board and it's initialized
    Position p0 = state.getBoard().getPawnPosition(0);
    TEST_ASSERT(p0.x == 4 && p0.y == 0, "Board accessible and Player 0 at start");
    
    // Modify board through state
    state.getBoard().setPawnPosition(0, 1, 1);
    Position p0_new = state.getBoard().getPawnPosition(0);
    TEST_ASSERT(p0_new.x == 1 && p0_new.y == 1, "Board modified through State");
    
    return 0;
}

int main() {
    int failures = 0;
    std::cout << "Starting State Unit Tests" << std::endl;
    
    failures += test_initialization();
    failures += test_player_switching();
    failures += test_board_integration();
    
    if (failures == 0) {
        std::cout << "\n================================" << std::endl;
        std::cout << "✅ All State tests passed!" << std::endl;
        std::cout << "================================" << std::endl;
        return 0;
    } else {
        std::cerr << "\n❌ " << failures << " test suite(s) failed." << std::endl;
        return 1;
    }
}
