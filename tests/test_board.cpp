#include <iostream>
#include <stdexcept>
#include "model/Board.hpp" // Include the Board class header (located in include/model/)

using namespace Quoridor;

// Simple assertion macro (simulating Catch2's REQUIRE)
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "[FAIL] Test line " << __LINE__ << ": " << message << std::endl; \
            return 1; \
        } \
        std::cout << "[PASS] " << message << std::endl; \
    } while(0)

// Simple exception assertion macro
#define TEST_ASSERT_THROWS(code, exceptionType, message) \
    do { \
        bool thrown = false; \
        try { code; } \
        catch (const exceptionType& e) { \
            thrown = true; \
        } \
        if (!thrown) { \
            std::cerr << "[FAIL] Test line " << __LINE__ << ": Expected exception " << #exceptionType << " but none was thrown. (" << message << ")" << std::endl; \
            return 1; \
        } \
        std::cout << "[PASS] Expected exception thrown: " << message << std::endl; \
    } while(0)


int test_board_initialization() {
    std::cout << "\n--- TEST: Board Initialization and Positions ---" << std::endl;
    Board board;

    // 1. Check the board dimension constant
    TEST_ASSERT(BOARD_SIZE == 9, "BOARD_SIZE is 9.");

    // 2. Check Player 0's starting position: (4, 0)
    Position p0_start = board.getPawnPosition(0);
    TEST_ASSERT(p0_start.x == 4 && p0_start.y == 0, "Pawn 0 starts at (4, 0).");

    // 3. Check Player 1's starting position: (4, 8)
    Position p1_start = board.getPawnPosition(1);
    TEST_ASSERT(p1_start.x == 4 && p1_start.y == 8, "Pawn 1 starts at (4, 8).");
    
    return 0;
}

int test_pawn_movement() {
    std::cout << "\n--- TEST: Pawn Movement ---" << std::endl;
    Board board;
    
    // 1. Move Player 0 to (4, 1)
    Position p0_new = {4, 1};
    board.movePawn(0, p0_new);
    Position p0_current = board.getPawnPosition(0);
    
    TEST_ASSERT(p0_current == p0_new, "Pawn 0 successfully moved to (4, 1).");
    
    // 2. Attempt to move out of bounds (9, 5), expecting a std::invalid_argument exception
    TEST_ASSERT_THROWS( 
        board.movePawn(0, {9, 5}),
        std::invalid_argument,
        "Move to (9, 5) throws invalid_argument."
    );
    
    // 3. Attempt to use an invalid player index (-1), expecting a std::out_of_range exception
    TEST_ASSERT_THROWS( 
        board.movePawn(-1, {4, 4}),
        std::out_of_range,
        "Move with player -1 throws out_of_range."
    );

    return 0;
}

int test_wall_placement() {
    std::cout << "\n--- TEST: Wall Placement ---" << std::endl;
    Board board;
    
    // 1. Place a valid horizontal wall (H)
    Wall wall1 = {{3, 3}, Orientation::Horizontal};
    board.placeWall(wall1);
    TEST_ASSERT(board.getWalls().size() == 1, "First wall placed successfully.");

    // 2. Place a valid vertical wall (V)
    Wall wall2 = {{0, 0}, Orientation::Vertical};
    board.placeWall(wall2);
    TEST_ASSERT(board.getWalls().size() == 2, "Second wall placed successfully.");
    
    // 3. Attempt to place a wall in an invalid area (8, 5) - wall grid max is 7
    TEST_ASSERT_THROWS(
        board.placeWall({{8, 5}, Orientation::Horizontal}),
        std::invalid_argument,
        "Placing wall at (8, 5) throws invalid_argument."
    );
    
    return 0;
}

int test_new_requirements() {
    std::cout << "\n--- TEST: New Requirements (setPawnPosition, isInBounds) ---" << std::endl;
    Board board;

    // 1. Test isInBounds
    TEST_ASSERT(Board::isInBounds(0, 0) == true, "isInBounds(0, 0) is true");
    TEST_ASSERT(Board::isInBounds(8, 8) == true, "isInBounds(8, 8) is true");
    TEST_ASSERT(Board::isInBounds(-1, 0) == false, "isInBounds(-1, 0) is false");
    TEST_ASSERT(Board::isInBounds(0, 9) == false, "isInBounds(0, 9) is false");

    // 2. Test setPawnPosition
    board.setPawnPosition(0, 2, 3); // Player 0 to row 2, col 3
    Position p0 = board.getPawnPosition(0);
    TEST_ASSERT(p0.x == 3 && p0.y == 2, "setPawnPosition(0, 2, 3) moved pawn to x=3, y=2");

    TEST_ASSERT_THROWS(
        board.setPawnPosition(0, 9, 9),
        std::invalid_argument,
        "setPawnPosition out of bounds throws invalid_argument"
    );

    return 0;
}

int main() {
    int failures = 0;
    std::cout << "Starting Board Unit Tests (C++17 mode)" << std::endl;

    failures += test_board_initialization();
    failures += test_pawn_movement();
    failures += test_wall_placement();
    failures += test_new_requirements();

    if (failures == 0) {
        std::cout << "\n================================" << std::endl;
        std::cout << "✅ All Board tests passed!" << std::endl;
        std::cout << "================================" << std::endl;
        return 0;
    } else {
        std::cout << "\n================================" << std::endl;
        std::cerr << "❌ " << failures << " test suite(s) failed." << std::endl;
        std::cout << "================================" << std::endl;
        return 1;
    }
}
