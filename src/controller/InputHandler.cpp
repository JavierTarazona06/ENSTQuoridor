#include "controller/InputHandler.hpp"
#include <iostream>
#include <optional>

namespace Quoridor {

InputHandler::InputHandler(Board& board, State& state, Rules& rules)
    : board(board), state(state), rules(rules) {}

void InputHandler::handleInput(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mousePressed = event.getIf<sf::Event::MouseButtonPressed>();
        if (mousePressed->button == sf::Mouse::Button::Left) {
            handleMouseClick(mousePressed->position.x, mousePressed->position.y);
        }
    }
}

bool InputHandler::pixelToGrid(int pixelX, int pixelY, int& row, int& col) const {
    float offsetX = Render2D::getGridOffsetX();
    float offsetY = Render2D::getGridOffsetY();
    float cellSize = Render2D::getCellSize();
    
    // Check if click is within the grid area
    if (pixelX < offsetX || pixelY < offsetY) {
        return false;
    }
    
    col = static_cast<int>((pixelX - offsetX) / cellSize);
    row = static_cast<int>((pixelY - offsetY) / cellSize);
    
    // Validate grid coordinates
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return true;
    }
    
    return false;
}

void InputHandler::handleMouseClick(int pixelX, int pixelY) {
    // If game is over, ignore input
    if (state.getGameStatus() != GameStatus::Playing) {
        return;
    }

    int row, col;
    if (!pixelToGrid(pixelX, pixelY, row, col)) {
        // Clicked outside the grid
        return;
    }

    int currentPlayer = state.getCurrentPlayer();
    Position clickedPos = {col, row}; // Board uses x=col, y=row
    std::optional<Position> selectedPos = state.getSelectedPawn();

    if (!selectedPos.has_value()) {
        // Try to select current player's pawn
        Position playerPos = board.getPawnPosition(currentPlayer);
        if (playerPos == clickedPos) {
            state.setSelectedPawn(clickedPos);
        }
    } else {
        // A pawn is already selected, try to move
        Position currentPos = selectedPos.value();
        
        // Check if we clicked on the same pawn (deselect)
        if (clickedPos == currentPos) {
            state.setSelectedPawn(std::nullopt);
            return;
        }

        // Check if move is valid
        if (Rules::isValidMove(board, currentPlayer, currentPos.y, currentPos.x, clickedPos.y, clickedPos.x)) {
            // Valid move
            board.movePawn(currentPlayer, clickedPos);
            
            // Check for victory
            if (Rules::checkVictory(board, currentPlayer)) {
                if (currentPlayer == 0) {
                    state.setGameStatus(GameStatus::Player1Won);
                } else {
                    state.setGameStatus(GameStatus::Player2Won);
                }
            } else {
                // Only switch player if no one has won yet
                state.switchPlayer();
            }
            
            state.setSelectedPawn(std::nullopt);
        } else {
            // Invalid move - just reject (do nothing)
            // If user clicked on their own pawn again (but not the selected one? impossible as only 1 pawn per player)
            // If we support clicking the pawn again to deselect (handled above)
        }
    }
}

} // namespace Quoridor
