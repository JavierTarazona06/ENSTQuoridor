#include <iostream>
#include "model/Rules.hpp"
#include "model/Board.hpp"

using namespace Quoridor;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL] Test line " << __LINE__ << ": " << message << std::endl; \
            return 1; \
        } \
        std::cout << "[PASS] " << message << std::endl; \
    } while(0)

int test_basic_movement() {
    std::cout << "\n--- TEST: Basic Movement Validation ---" << std::endl;
    Board board;
    // Player 0 starts at (4, 0)
    
    // Valid move: down to (4, 1)
    TEST_ASSERT(Rules::isValidMove(board, 0, 0, 4, 1, 4), "Valid move (0,4) -> (1,4)");
    
    // Valid move: left to (3, 0) (assuming no walls/pawns, but board has P0 at 4,0)
    // Actually, Board state is fixed. P0 is at (4,0). P1 at (4,8).
    // isValidMove parameters are explicit from/to, but it checks against board state for collisions.
    // The board state itself has P0 at (4,0). So if we say "move P0 from 4,0 to 3,0", it's valid.
    
    TEST_ASSERT(Rules::isValidMove(board, 0, 0, 4, 0, 3), "Valid move (0,4) -> (0,3)");
    
    // Invalid: Diagonal
    TEST_ASSERT(!Rules::isValidMove(board, 0, 0, 4, 1, 5), "Invalid diagonal move");
    
    // Invalid: Jump (2 steps)
    TEST_ASSERT(!Rules::isValidMove(board, 0, 0, 4, 2, 4), "Invalid jump (2 steps)");
    
    // Invalid: Out of bounds
    TEST_ASSERT(!Rules::isValidMove(board, 0, 0, 4, -1, 4), "Invalid out of bounds");

    return 0;
}

int test_collision() {
    std::cout << "\n--- TEST: Collision Validation ---" << std::endl;
    Board board;
    
    // Place P1 adjacent to P0 for collision test
    // P0 at (4, 0). Let's put P1 at (4, 1).
    board.setPawnPosition(1, 1, 4); // row 1, col 4
    
    // P0 tries to move to (4, 1) which is occupied by P1
    TEST_ASSERT(!Rules::isValidMove(board, 0, 0, 4, 1, 4), "Cannot move to occupied cell");
    
    return 0;
}

int test_wall_blocking() {
    std::cout << "\n--- TEST: Wall Blocking ---" << std::endl;
    Board board;
    
    // P0 at (4, 0). Place horizontal wall at (4, 0).
    // Wall at (4, 0) Horizontal spans cols 4 and 5, between rows 0 and 1.
    // This should block P0 moving from (4, 0) to (4, 1).
    
    Wall hWall = {{4, 0}, Orientation::Horizontal};
    board.placeWall(hWall);
    
    TEST_ASSERT(!Rules::isValidMove(board, 0, 0, 4, 1, 4), "Horizontal wall blocks movement down");
    
    // Place vertical wall at (3, 0).
    // Wall at (3, 0) Vertical spans rows 0 and 1, between cols 3 and 4.
    // This should block P0 moving from (4, 0) to (3, 0).
    
    Wall vWall = {{3, 0}, Orientation::Vertical};
    board.placeWall(vWall);
    
    TEST_ASSERT(!Rules::isValidMove(board, 0, 0, 4, 0, 3), "Vertical wall blocks movement left");
    
    // Movement to right (5, 0) should still be valid
    TEST_ASSERT(Rules::isValidMove(board, 0, 0, 4, 0, 5), "Movement right is unblocked");
    
    return 0;
}

int test_victory() {
    std::cout << "\n--- TEST: Victory Condition ---" << std::endl;
    Board board;
    
    // Player 0 at (4, 0) - Not won
    TEST_ASSERT(!Rules::checkVictory(board, 0), "Player 0 at start not won");
    
    // Move Player 0 to row 8
    board.setPawnPosition(0, 8, 4);
    TEST_ASSERT(Rules::checkVictory(board, 0), "Player 0 at row 8 has won");
    
    // Move Player 0 back to 7
    board.setPawnPosition(0, 7, 4);
    TEST_ASSERT(!Rules::checkVictory(board, 0), "Player 0 at row 7 not won");

    // Player 1 at row 0 wins
    board.setPawnPosition(1, 0, 4);
    TEST_ASSERT(Rules::checkVictory(board, 1), "Player 1 at row 0 has won");
    
    return 0;
}

int main() {
    int failures = 0;
    std::cout << "Starting Rules Unit Tests" << std::endl;
    
    failures += test_basic_movement();
    failures += test_collision();
    failures += test_wall_blocking();
    failures += test_victory();
    
    if (failures == 0) {
        std::cout << "\n================================" << std::endl;
        std::cout << "✅ All Rules tests passed!" << std::endl;
        std::cout << "================================" << std::endl;
        return 0;
    } else {
        std::cerr << "\n❌ " << failures << " test suite(s) failed." << std::endl;
        return 1;
    }
}
