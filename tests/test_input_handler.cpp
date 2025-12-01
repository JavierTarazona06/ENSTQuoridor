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
    State state(board);
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
    State state(board);
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
    State state(board);
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
    State state(board);
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
