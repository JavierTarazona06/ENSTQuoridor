#include <catch2/catch_test_macros.hpp>
#include "model/Board.hpp"
#include "model/Rules.hpp"

using namespace Quoridor;

TEST_CASE("Straight Jump Rules", "[rules][jump]") {
    Board board;
    board.resetGame(); // P0 at (4,0), P1 at (4,8)

    // Setup: Player 0 at (4,4), Player 1 at (4,5) (below P0)
    board.setPawnPosition(0, 4, 4);
    board.setPawnPosition(1, 5, 4);

    SECTION("Valid Straight Jump Over Opponent") {
        // P0 jumps over P1 to (6,4)
        bool isValid = Rules::isValidMove(board, 0, 4, 4, 6, 4);
        REQUIRE(isValid == true);
    }

    SECTION("Invalid Straight Jump: No Opponent") {
        // Move P1 away
        board.setPawnPosition(1, 6, 6);
        // Try to jump from (4,4) to (6,4) without opponent
        bool isValid = Rules::isValidMove(board, 0, 4, 4, 6, 4);
        REQUIRE(isValid == false);
    }

    SECTION("Invalid Straight Jump: Destination Occupied") {
        // Hypothetical 3rd pawn or obstacle? 
        // Or if we somehow try to jump on top of someone. 
        // But with 2 players, if P1 is at (4,5), P0 jumps to (4,6).
        // Let's create a scenario where jump lands out of bounds or blocked.
        
        // Setup: P0 at (4,7), P1 at (4,8). P0 jumps to (4,9) -> Out of bounds
        board.setPawnPosition(0, 7, 4);
        board.setPawnPosition(1, 8, 4);
        
        // Rules::isValidMove checks bounds first, but we are testing jump logic integration
        bool isValid = Rules::isValidMove(board, 0, 7, 4, 9, 4);
        REQUIRE(isValid == false);
    }

    SECTION("Invalid Straight Jump: Blocked by Wall behind opponent") {
        // P0(4,4), P1(4,5). Wall between (4,5) and (4,6)
        // Horizontal wall at (4,5) spans (4,5) and (5,5)? No, walls are between cells.
        // Wall coord (x,y) top-left.
        // To block (4,5)->(4,6), we need Horizontal wall at y=5. x=4.
        // Wall at (4,5) Horizontal blocks (4,5)->(4,6) and (5,5)->(5,6)
        
        Wall w{{4, 5}, Orientation::Horizontal}; 
        board.placeWall(w, 1);
        
        // Check straight jump
        bool isValid = Rules::isValidMove(board, 0, 4, 4, 6, 4);
        REQUIRE(isValid == false);
    }

    SECTION("Invalid Straight Jump: Blocked by Wall between players") {
        // P0(4,4), P1(4,5). Wall between P0 and P1.
        // Wall between (4,4) and (4,5).
        // Horizontal wall at (4,4).
        
        Wall w{{4, 4}, Orientation::Horizontal};
        board.placeWall(w, 1);
        
        // Check straight jump
        bool isValid = Rules::isValidMove(board, 0, 4, 4, 6, 4);
        REQUIRE(isValid == false);
        
        // P0 shouldn't be able to move to P1's position either (simple move)
        // Rules::isValidMove handles simple move dist=1.
        // But jump is dist=2.
        // The jump logic checks both steps.
    }
}

TEST_CASE("Diagonal Jump Rules", "[rules][jump]") {
    Board board;
    board.resetGame();

    // Setup: P0 at (4,4), P1 at (4,5)
    board.setPawnPosition(0, 4, 4);
    board.setPawnPosition(1, 5, 4);

    SECTION("Valid Diagonal Jump when Straight is Blocked by Wall") {
        // Block straight jump with wall behind P1
        Wall w{{4, 5}, Orientation::Horizontal}; 
        board.placeWall(w, 1); // Blocks (4,5)->(4,6)

        // Try diagonal jumps
        // Down-Left: (4,4) -> (4,5)(opp) -> (5,3) ? No.
        // Opponent is at (4,5).
        // Diagonal jump is to (5,3) or (5,5).
        // Coordinates: (row, col). 
        // P0(row=4, col=4). P1(row=5, col=4).
        // Straight jump: (row=6, col=4). Blocked.
        // Diagonal options: (row=5, col=3) [Left of P1] and (row=5, col=5) [Right of P1].
        
        // Try move to (5,3)
        bool validLeft = Rules::isValidMove(board, 0, 4, 4, 5, 3);
        REQUIRE(validLeft == true);

        // Try move to (5,5)
        bool validRight = Rules::isValidMove(board, 0, 4, 4, 5, 5);
        REQUIRE(validRight == true);
    }
    
    SECTION("Valid Diagonal Jump when Straight is Blocked by Board Edge") {
        // Setup: P0 at (7,4), P1 at (8,4).
        // Straight jump to (9,4) is out of bounds.
        board.setPawnPosition(0, 7, 4);
        board.setPawnPosition(1, 8, 4);
        
        // Diagonal to (8,3) or (8,5)
        bool validLeft = Rules::isValidMove(board, 0, 7, 4, 8, 3);
        REQUIRE(validLeft == true);
        
        bool validRight = Rules::isValidMove(board, 0, 7, 4, 8, 5);
        REQUIRE(validRight == true);
    }

    SECTION("Invalid Diagonal Jump: Straight Path Not Blocked") {
        // P0(4,4), P1(4,5). No walls.
        // Straight jump (6,4) is available.
        // Diagonal (5,3) should be invalid.
        bool validDiagonal = Rules::isValidMove(board, 0, 4, 4, 5, 3);
        REQUIRE(validDiagonal == false);
    }

    SECTION("Invalid Diagonal Jump: Blocked by Wall") {
        // P0(4,4), P1(4,5). Wall behind P1 (Horizontal at 4,5) blocks straight.
        // Add Vertical wall at (3,5) to block (4,5)->(5,3) ?
        // Wall coords are top-left of the 4 cells around intersection.
        // Vertical wall at (3,5) blocks col 3 and 4? No.
        // Vertical wall at (x,y) blocks (x,y)<->(x+1,y) and (x,y+1)<->(x+1,y+1).
        // We want to block move from P1(row=5, col=4) to Left(row=5, col=3).
        // So we need Vertical wall between col 3 and 4 at row 5.
        // Vertical wall at (3,5) blocks (3,5)<->(4,5)
        
        Wall wStraight{{4, 5}, Orientation::Horizontal}; 
        board.placeWall(wStraight, 1); // Blocks straight jump
        
        Wall wDiag{{3, 5}, Orientation::Vertical};
        board.placeWall(wDiag, 0); // Blocks movement to left (col 3) from P1 (col 4) at row 5?
        // Wait, Wall at (3,5) vertical blocks:
        // (3,5) <-> (4,5)  (row 5, col 3 <-> row 5, col 4) -> Yes, this is the path!
        // (3,6) <-> (4,6)  (row 6)

        bool validLeft = Rules::isValidMove(board, 0, 4, 4, 5, 3);
        REQUIRE(validLeft == false);
        
        // Right side should still be open
        bool validRight = Rules::isValidMove(board, 0, 4, 4, 5, 5);
        REQUIRE(validRight == true);
    }
}
