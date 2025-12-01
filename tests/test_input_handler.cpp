#include <catch2/catch_test_macros.hpp>
#include <SFML/Window/Event.hpp>
#include "controller/InputHandler.hpp"
#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"
#include "view/Render2D.hpp"

using namespace Quoridor;

// Helper to create a mouse click event
sf::Event createClickEvent(int x, int y) {
    sf::Event::MouseButtonPressed mouseEvent;
    mouseEvent.button = sf::Mouse::Button::Left;
    mouseEvent.position = {x, y};
    return sf::Event(mouseEvent);
}

// Helper to calculate pixel coordinates from grid coordinates
// Reverse of pixelToGrid logic
// x = offset + col * size + size/2 (center)
void gridToPixel(int row, int col, int& x, int& y) {
    x = static_cast<int>(Render2D::getGridOffsetX() + col * Render2D::getCellSize() + Render2D::getCellSize() / 2);
    y = static_cast<int>(Render2D::getGridOffsetY() + row * Render2D::getCellSize() + Render2D::getCellSize() / 2);
}

TEST_CASE("InputHandler pixel to grid conversion", "[input][conversion]") {
    Board board;
    State state;
    Rules rules;
    InputHandler inputHandler(board, state, rules);
    
    // We can't test pixelToGrid directly as it is private, 
    // but we can verify the logic works by selecting a pawn.
    // Player 0 starts at (4, 0) -> row 0, col 4.
    
    int x, y;
    gridToPixel(0, 4, x, y);
    
    sf::RenderWindow window; // Dummy window, not actually used by handleInput logic we are testing
    
    // First click: Select pawn at (0, 4)
    inputHandler.handleInput(createClickEvent(x, y), window);
    
    // To verify selection, we try to move it to a valid spot (0, 3) or (0, 5) or (1, 4).
    // (1, 4) is directly below.
    
    int targetX, targetY;
    gridToPixel(1, 4, targetX, targetY);
    
    // Second click: Move to (1, 4)
    inputHandler.handleInput(createClickEvent(targetX, targetY), window);
    
    // Check if pawn moved
    Position newPos = board.getPawnPosition(0);
    REQUIRE(newPos.x == 4);
    REQUIRE(newPos.y == 1);
}

TEST_CASE("InputHandler invalid moves", "[input][logic]") {
    Board board;
    State state;
    Rules rules;
    InputHandler inputHandler(board, state, rules);
    
    // Player 0 at (4, 0)
    int x, y;
    gridToPixel(0, 4, x, y);
    
    sf::RenderWindow window;
    
    // Select pawn
    inputHandler.handleInput(createClickEvent(x, y), window);
    
    // Try to move to invalid position (e.g. (3, 4) - too far)
    int targetX, targetY;
    gridToPixel(3, 4, targetX, targetY); // 3 rows down
    
    inputHandler.handleInput(createClickEvent(targetX, targetY), window);
    
    // Pawn should not have moved
    Position pos = board.getPawnPosition(0);
    REQUIRE(pos.x == 4);
    REQUIRE(pos.y == 0);
    
    // Player should still be 0 (turn didn't switch)
    REQUIRE(state.getCurrentPlayer() == 0);
}

TEST_CASE("InputHandler click outside grid", "[input][bounds]") {
    Board board;
    State state;
    Rules rules;
    InputHandler inputHandler(board, state, rules);
    
    sf::RenderWindow window;
    
    // Click at (0, 0) which should be outside grid (offset is 180, 180)
    inputHandler.handleInput(createClickEvent(0, 0), window);
    
    // Nothing should happen, no crash
    REQUIRE(state.getCurrentPlayer() == 0);
}

TEST_CASE("InputHandler deselect logic", "[input][logic]") {
    Board board;
    State state;
    Rules rules;
    InputHandler inputHandler(board, state, rules);
    
    // Player 0 at (4, 0)
    int x, y;
    gridToPixel(0, 4, x, y);
    sf::RenderWindow window;
    
    // Select pawn
    inputHandler.handleInput(createClickEvent(x, y), window);
    
    // Click on same pawn again to deselect
    inputHandler.handleInput(createClickEvent(x, y), window);
    
    // Try to move (should fail because no pawn selected)
    int targetX, targetY;
    gridToPixel(1, 4, targetX, targetY);
    inputHandler.handleInput(createClickEvent(targetX, targetY), window);
    
    // Pawn should not have moved
    Position pos = board.getPawnPosition(0);
    REQUIRE(pos.x == 4);
    REQUIRE(pos.y == 0);
}

// Helper to create a key pressed event
sf::Event createKeyEvent(sf::Keyboard::Key key) {
    sf::Event::KeyPressed keyEvent;
    keyEvent.code = key;
    keyEvent.alt = false;
    keyEvent.control = false;
    keyEvent.shift = false;
    keyEvent.system = false;
    return sf::Event(keyEvent);
}

TEST_CASE("InputHandler R key resets game when finished", "[input][reset]") {
    Board board;
    State state;
    Rules rules;
    InputHandler inputHandler(board, state, rules);
    sf::RenderWindow window;
    
    SECTION("R key resets game when Player1 won") {
        // Set game status to Player1Won
        state.setGameStatus(GameStatus::Player1Won);
        
        // Move pawn to a different position to verify reset
        board.movePawn(0, {5, 3});
        board.movePawn(1, {2, 6});
        state.switchPlayer(); // Player 1's turn
        
        // Verify game is finished
        REQUIRE(state.getGameStatus() == GameStatus::Player1Won);
        REQUIRE(board.getPawnPosition(0).x == 5);
        REQUIRE(board.getPawnPosition(0).y == 3);
        
        // Press R key
        inputHandler.handleInput(createKeyEvent(sf::Keyboard::Key::R), window);
        
        // Verify game is reset
        REQUIRE(state.getGameStatus() == GameStatus::Playing);
        REQUIRE(state.getCurrentPlayer() == 0);
        
        // Verify pawns are back to initial positions
        Position p0 = board.getPawnPosition(0);
        Position p1 = board.getPawnPosition(1);
        REQUIRE(p0.x == BOARD_SIZE / 2);
        REQUIRE(p0.y == 0);
        REQUIRE(p1.x == BOARD_SIZE / 2);
        REQUIRE(p1.y == BOARD_SIZE - 1);
    }
    
    SECTION("R key resets game when Player2 won") {
        // Set game status to Player2Won
        state.setGameStatus(GameStatus::Player2Won);
        
        // Verify game is finished
        REQUIRE(state.getGameStatus() == GameStatus::Player2Won);
        
        // Press R key
        inputHandler.handleInput(createKeyEvent(sf::Keyboard::Key::R), window);
        
        // Verify game is reset
        REQUIRE(state.getGameStatus() == GameStatus::Playing);
        REQUIRE(state.getCurrentPlayer() == 0);
    }
    
    SECTION("R key does NOT reset game while playing") {
        // Game is already in Playing state by default
        REQUIRE(state.getGameStatus() == GameStatus::Playing);
        
        // Move pawn to a different position
        board.movePawn(0, {5, 3});
        state.switchPlayer();
        
        // Press R key
        inputHandler.handleInput(createKeyEvent(sf::Keyboard::Key::R), window);
        
        // Game should NOT be reset (pawn should still be at moved position)
        Position p0 = board.getPawnPosition(0);
        REQUIRE(p0.x == 5);
        REQUIRE(p0.y == 3);
        
        // Player should still be 1 (not reset to 0)
        REQUIRE(state.getCurrentPlayer() == 1);
    }
}
