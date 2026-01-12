#include "controller/InputHandler.hpp"
#include <iostream>
#include <optional>

namespace Quoridor {

InputHandler::InputHandler(Board& board, State& state, Rules& rules)
    : board(board), state(state), rules(rules) {}

void InputHandler::handleInput(const sf::Event& event, const sf::RenderWindow& window) {
    // If game is over, only allow reset
    if (state.getGameStatus() != GameStatus::Playing) {
        if (event.is<sf::Event::KeyPressed>()) {
            if (event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                board.resetGame();
                state.resetGame();
                currentMode = InputMode::MovePawn;
            }
        }
        return;
    }
    
    // Handle key pressed events - toggle between modes
    if (event.is<sf::Event::KeyPressed>()) {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        
        // 'W' key - toggle wall placement mode
        if (keyEvent->code == sf::Keyboard::Key::W) {
            if (currentMode == InputMode::MovePawn) {
                currentMode = InputMode::PlaceWall;
                state.setSelectedPawn(std::nullopt); // Deselect any selected pawn
                state.setPreviewWall(std::nullopt);
                std::cout << "Wall placement mode activated (Press W to return to movement)" << std::endl;
            } else {
                currentMode = InputMode::MovePawn;
                state.setPreviewWall(std::nullopt);
                std::cout << "Movement mode activated (Press W to place walls)" << std::endl;
            }
        }
        
        // 'O' key - rotate wall (horizontal <-> vertical)
        if (keyEvent->code == sf::Keyboard::Key::O && currentMode == InputMode::PlaceWall) {
            auto preview = state.getPreviewWall();
            if (preview.has_value()) {
                Wall rotatedWall = preview.value();
                rotatedWall.orientation = (rotatedWall.orientation == Orientation::Horizontal) 
                    ? Orientation::Vertical 
                    : Orientation::Horizontal;
                state.setPreviewWall(rotatedWall);
                std::cout << "Wall rotated to " << (rotatedWall.orientation == Orientation::Horizontal ? "Horizontal" : "Vertical") << std::endl;
            }
        }
    }
    
    // Handle mouse movement for wall preview
    if (event.is<sf::Event::MouseMoved>()) {
        const auto& mouseMoved = event.getIf<sf::Event::MouseMoved>();
        if (currentMode == InputMode::PlaceWall) {
            handleMouseMove(mouseMoved->position.x, mouseMoved->position.y);
        }
    }

    // Handle mouse button pressed events
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

void InputHandler::handleMouseMove(int pixelX, int pixelY) {
    int row, col;
    if (!pixelToGrid(pixelX, pixelY, row, col)) {
        state.setPreviewWall(std::nullopt);
        return;
    }
    
    // Create a preview wall at the clicked position
    // Default to horizontal orientation
    Wall previewWall;
    previewWall.pos = {col, row};
    previewWall.orientation = Orientation::Horizontal;
    
    // Check if this position is valid for wall placement (basic bounds check)
    if (col >= 0 && col < (BOARD_SIZE - 1) && row >= 0 && row < (BOARD_SIZE - 1)) {
        state.setPreviewWall(previewWall);
    } else {
        state.setPreviewWall(std::nullopt);
    }
}

void InputHandler::handleMouseClick(int pixelX, int pixelY) {
    int row, col;
    if (!pixelToGrid(pixelX, pixelY, row, col)) {
        // Clicked outside the grid
        return;
    }

    int currentPlayer = state.getCurrentPlayer();

    if (currentMode == InputMode::PlaceWall) {
        // Wall placement mode
        Wall newWall;
        newWall.pos = {col, row};
        
        // Get orientation from preview
        auto preview = state.getPreviewWall();
        if (preview.has_value()) {
            newWall.orientation = preview.value().orientation;
        } else {
            newWall.orientation = Orientation::Horizontal;
        }
        
        // Validate wall placement
        if (rules.isValidWallPlacement(board, newWall, currentPlayer)) {
            // Place the wall (this also decrements walls remaining)
            board.placeWall(newWall, currentPlayer);
            
            std::cout << "Wall placed successfully! Walls remaining: " << board.getWallsRemaining(currentPlayer) << std::endl;
            
            // Switch to next player
            state.switchPlayer();
            state.setPreviewWall(std::nullopt);
            
            // Auto-switch back to movement mode
            currentMode = InputMode::MovePawn;
        } else {
            std::cout << "Invalid wall placement!" << std::endl;
            // Keep the preview, let user try again
        }
    } else {
        // Pawn movement mode
        Position clickedPos = {col, row}; // Board uses x=col, y=row
        std::optional<Position> selectedPos = state.getSelectedPawn();

        if (!selectedPos.has_value()) {
            // Try to select current player's pawn
            Position playerPos = board.getPawnPosition(currentPlayer);
            if (playerPos == clickedPos) {
                state.setSelectedPawn(clickedPos);
                std::cout << "Pawn selected. Click destination or press W to place wall." << std::endl;
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
            if (rules.isValidMove(board, currentPlayer, currentPos.y, currentPos.x, clickedPos.y, clickedPos.x)) {
                // Valid move
                board.movePawn(currentPlayer, clickedPos);
                
                std::cout << "Pawn moved successfully!" << std::endl;
                
                // Check for victory
                if (rules.checkVictory(board, currentPlayer)) {
                    if (currentPlayer == 0) {
                        state.setGameStatus(GameStatus::Player1Won);
                        std::cout << "Player 1 (Top) wins!" << std::endl;
                    } else {
                        state.setGameStatus(GameStatus::Player2Won);
                        std::cout << "Player 2 (Bottom) wins!" << std::endl;
                    }
                } else {
                    // Only switch player if no one has won yet
                    state.switchPlayer();
                }
                
                state.setSelectedPawn(std::nullopt);
            } else {
                std::cout << "Invalid move!" << std::endl;
                // Invalid move - just reject (do nothing)
            }
        }
    }
}

} // namespace Quoridor
