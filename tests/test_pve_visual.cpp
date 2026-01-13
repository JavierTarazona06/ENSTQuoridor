/**
 * @file test_pve_visual.cpp
 * @brief Visual PVE (Player vs AI) Game Test Program
 * 
 * This is a standalone executable that reuses the existing UI components
 * but replaces Player 2 with AI control.
 * 
 * This test does NOT modify any existing project code!
 * 
 * Controls:
 * - Player 1 (Human): Click to move pawn, press W to place wall, press O to rotate wall
 * - Player 2 (AI): Responds automatically after human moves
 * - Press 1-4 to select AI difficulty
 * - Press R to reset game
 * 
 * @note This file serves as a verification tool for the AI engine
 *       without requiring Controller/View integration.
 */

#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"
#include "model/AI.hpp"
#include "view/Render2D.hpp"
#include "view/GameView.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace Quoridor;

// ============================================================================
// PVEGame - Standalone game controller with AI support
// ============================================================================

class PVEGame {
public:
    PVEGame() 
        : board(), state(), rules(), renderer(), gameView(),
          currentMode(InputMode::MovePawn),
          aiDifficulty(Difficulty::Normal),
          aiThinking(false)
    {
        printWelcomeMessage();
    }

    /**
     * @brief Main game loop
     */
    void run() {
        while (renderer.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    /**
     * @brief Input mode enumeration
     */
    enum class InputMode {
        MovePawn,
        PlaceWall
    };

    /**
     * @brief Print welcome message and controls guide
     */
    void printWelcomeMessage() {
        std::cout << "\n";
        std::cout << "========================================\n";
        std::cout << "       Quoridor PVE Visual Test\n";
        std::cout << "========================================\n";
        std::cout << "\n";
        std::cout << "Player 1 (Human) - White pawn - Move downward\n";
        std::cout << "Player 2 (AI)    - Black pawn - Move upward\n";
        std::cout << "\n";
        std::cout << "Controls:\n";
        std::cout << "  Click pawn -> Select -> Click destination to move\n";
        std::cout << "  W key: Toggle wall placement mode\n";
        std::cout << "  O key: Rotate wall orientation\n";
        std::cout << "  R key: Reset game\n";
        std::cout << "\n";
        std::cout << "AI Difficulty (press number key to change):\n";
        std::cout << "  1: Easy   (Random moves)\n";
        std::cout << "  2: Normal (Minimax depth 2)\n";
        std::cout << "  3: Hard   (Minimax depth 3)\n";
        std::cout << "  4: Hell   (Minimax depth 5)\n";
        std::cout << "\n";
        std::cout << "Current AI difficulty: Normal\n";
        std::cout << "========================================\n";
    }

    /**
     * @brief Process all pending SFML events
     */
    void processEvents() {
        auto event = renderer.pollEvent();
        while (event.has_value()) {
            if (event->is<sf::Event::Closed>()) {
                renderer.close();
            } else {
                handleInput(*event);
            }
            event = renderer.pollEvent();
        }
    }

    /**
     * @brief Update game state, execute AI move if it's AI's turn
     */
    void update() {
        // If it's AI's turn and game is in progress, execute AI move
        if (state.getCurrentPlayer() == 1 && 
            state.getGameStatus() == GameStatus::Playing &&
            !aiThinking) {
            
            aiThinking = true;
            
            // Small delay to let player see the board state
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            
            executeAIMove();
            
            aiThinking = false;
        }
    }

    /**
     * @brief Render the current game state
     */
    void render() {
        renderer.clear();
        gameView.render(renderer, board, state);
        renderer.display();
    }

    /**
     * @brief Handle user input events
     * @param event SFML event to process
     */
    void handleInput(const sf::Event& event) {
        // Ignore input during AI's turn
        if (state.getCurrentPlayer() == 1 && state.getGameStatus() == GameStatus::Playing) {
            return;
        }

        // Only allow reset when game is over
        if (state.getGameStatus() != GameStatus::Playing) {
            if (event.is<sf::Event::KeyPressed>()) {
                if (event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                    resetGame();
                }
            }
            return;
        }

        // Handle keyboard input
        if (event.is<sf::Event::KeyPressed>()) {
            handleKeyPress(event);
        }

        // Handle mouse movement for wall preview
        if (event.is<sf::Event::MouseMoved>()) {
            const auto& mouseMoved = event.getIf<sf::Event::MouseMoved>();
            if (currentMode == InputMode::PlaceWall) {
                handleMouseMove(mouseMoved->position.x, mouseMoved->position.y);
            }
        }

        // Handle mouse click
        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto& mousePressed = event.getIf<sf::Event::MouseButtonPressed>();
            if (mousePressed->button == sf::Mouse::Button::Left) {
                handleMouseClick(mousePressed->position.x, mousePressed->position.y);
            }
        }
    }

    /**
     * @brief Handle keyboard key press events
     * @param event Key press event
     */
    void handleKeyPress(const sf::Event& event) {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        
        // W key - Toggle wall placement mode
        if (keyEvent->code == sf::Keyboard::Key::W) {
            if (currentMode == InputMode::MovePawn) {
                currentMode = InputMode::PlaceWall;
                state.setSelectedPawn(std::nullopt);
                state.setPreviewWall(std::nullopt);
                std::cout << "Wall placement mode (press W to return to movement)\n";
            } else {
                currentMode = InputMode::MovePawn;
                state.setPreviewWall(std::nullopt);
                std::cout << "Movement mode (press W to place walls)\n";
            }
        }
        
        // O key - Rotate wall
        if (keyEvent->code == sf::Keyboard::Key::O && currentMode == InputMode::PlaceWall) {
            auto preview = state.getPreviewWall();
            if (preview.has_value()) {
                Wall rotatedWall = preview.value();
                rotatedWall.orientation = (rotatedWall.orientation == Orientation::Horizontal) 
                    ? Orientation::Vertical 
                    : Orientation::Horizontal;
                state.setPreviewWall(rotatedWall);
            }
        }
        
        // R key - Reset game
        if (keyEvent->code == sf::Keyboard::Key::R) {
            resetGame();
        }
        
        // Number keys 1-4 - Select AI difficulty
        if (keyEvent->code == sf::Keyboard::Key::Num1) {
            aiDifficulty = Difficulty::Easy;
            std::cout << "AI difficulty: Easy\n";
        }
        if (keyEvent->code == sf::Keyboard::Key::Num2) {
            aiDifficulty = Difficulty::Normal;
            std::cout << "AI difficulty: Normal\n";
        }
        if (keyEvent->code == sf::Keyboard::Key::Num3) {
            aiDifficulty = Difficulty::Hard;
            std::cout << "AI difficulty: Hard\n";
        }
        if (keyEvent->code == sf::Keyboard::Key::Num4) {
            aiDifficulty = Difficulty::Hell;
            std::cout << "AI difficulty: Hell\n";
        }
    }

    /**
     * @brief Convert pixel coordinates to grid coordinates
     * @param pixelX X pixel coordinate
     * @param pixelY Y pixel coordinate
     * @param row Output row index
     * @param col Output column index
     * @return true if coordinates are within the grid
     */
    bool pixelToGrid(int pixelX, int pixelY, int& row, int& col) const {
        float offsetX = Render2D::getGridOffsetX();
        float offsetY = Render2D::getGridOffsetY();
        float cellSize = Render2D::getCellSize();
        
        if (pixelX < offsetX || pixelY < offsetY) {
            return false;
        }
        
        col = static_cast<int>((pixelX - offsetX) / cellSize);
        row = static_cast<int>((pixelY - offsetY) / cellSize);
        
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            return true;
        }
        
        return false;
    }

    /**
     * @brief Handle mouse movement for wall preview
     * @param pixelX X pixel coordinate
     * @param pixelY Y pixel coordinate
     */
    void handleMouseMove(int pixelX, int pixelY) {
        int row, col;
        if (!pixelToGrid(pixelX, pixelY, row, col)) {
            state.setPreviewWall(std::nullopt);
            return;
        }
        
        Wall previewWall;
        previewWall.pos = {col, row};
        previewWall.orientation = Orientation::Horizontal;
        
        // Preserve current preview wall orientation
        auto currentPreview = state.getPreviewWall();
        if (currentPreview.has_value()) {
            previewWall.orientation = currentPreview->orientation;
        }
        
        if (col >= 0 && col < (BOARD_SIZE - 1) && row >= 0 && row < (BOARD_SIZE - 1)) {
            state.setPreviewWall(previewWall);
        } else {
            state.setPreviewWall(std::nullopt);
        }
    }

    /**
     * @brief Handle mouse click for pawn movement or wall placement
     * @param pixelX X pixel coordinate
     * @param pixelY Y pixel coordinate
     */
    void handleMouseClick(int pixelX, int pixelY) {
        int row, col;
        if (!pixelToGrid(pixelX, pixelY, row, col)) {
            return;
        }

        int currentPlayer = state.getCurrentPlayer();

        if (currentMode == InputMode::PlaceWall) {
            handleWallPlacement(col, row, currentPlayer);
        } else {
            handlePawnMovement(col, row, currentPlayer);
        }
    }

    /**
     * @brief Handle wall placement logic
     * @param col Column index
     * @param row Row index
     * @param currentPlayer Current player index
     */
    void handleWallPlacement(int col, int row, int currentPlayer) {
        Wall newWall;
        newWall.pos = {col, row};
        
        auto preview = state.getPreviewWall();
        if (preview.has_value()) {
            newWall.orientation = preview.value().orientation;
        } else {
            newWall.orientation = Orientation::Horizontal;
        }
        
        if (rules.isValidWallPlacement(board, newWall, currentPlayer)) {
            board.placeWall(newWall, currentPlayer);
            std::cout << "Player placed a wall! Remaining: " << board.getWallsRemaining(currentPlayer) << "\n";
            
            state.switchPlayer();
            state.setPreviewWall(std::nullopt);
            currentMode = InputMode::MovePawn;
        } else {
            std::cout << "Invalid wall position!\n";
        }
    }

    /**
     * @brief Handle pawn movement logic
     * @param col Column index
     * @param row Row index
     * @param currentPlayer Current player index
     */
    void handlePawnMovement(int col, int row, int currentPlayer) {
        Position clickedPos = {col, row};
        std::optional<Position> selectedPos = state.getSelectedPawn();

        if (!selectedPos.has_value()) {
            // Try to select current player's pawn
            Position playerPos = board.getPawnPosition(currentPlayer);
            if (playerPos == clickedPos) {
                state.setSelectedPawn(clickedPos);
                std::cout << "Pawn selected. Click destination to move.\n";
            }
        } else {
            Position currentPos = selectedPos.value();
            
            // Clicking on the same pawn deselects it
            if (clickedPos == currentPos) {
                state.setSelectedPawn(std::nullopt);
                return;
            }

            // Try to move to clicked position
            if (rules.isValidMove(board, currentPlayer, currentPos.y, currentPos.x, clickedPos.y, clickedPos.x)) {
                board.movePawn(currentPlayer, clickedPos);
                std::cout << "Player moved to (" << clickedPos.x << "," << clickedPos.y << ")\n";
                
                // Check for victory
                if (rules.checkVictory(board, currentPlayer)) {
                    state.setGameStatus(GameStatus::Player1Won);
                    std::cout << "\n** Player 1 (Human) wins! **\n";
                    std::cout << "Press R to restart\n";
                } else {
                    state.switchPlayer();
                }
                
                state.setSelectedPawn(std::nullopt);
            } else {
                std::cout << "Invalid move!\n";
            }
        }
    }

    /**
     * @brief Execute AI move using the configured difficulty
     */
    void executeAIMove() {
        std::cout << ">>> AI thinking...\n";
        
        AI ai;
        Move aiMove = ai.getBestMove(board, state, aiDifficulty);
        
        if (aiMove.isPawnMove()) {
            board.movePawn(1, aiMove.pawnDest);
            std::cout << ">>> AI moved to (" << aiMove.pawnDest.x << "," << aiMove.pawnDest.y << ")\n";
        } else if (aiMove.isWallPlacement()) {
            board.placeWall(aiMove.wall, 1);
            std::cout << ">>> AI placed " 
                      << (aiMove.wall.orientation == Orientation::Horizontal ? "horizontal" : "vertical")
                      << " wall at (" << aiMove.wall.pos.x << "," << aiMove.wall.pos.y << ")\n";
        }
        
        // Check if AI wins
        if (rules.checkVictory(board, 1)) {
            state.setGameStatus(GameStatus::Player2Won);
            std::cout << "\n** Player 2 (AI) wins! **\n";
            std::cout << "Press R to restart\n";
        } else {
            state.switchPlayer();
        }
    }

    /**
     * @brief Reset game to initial state
     */
    void resetGame() {
        board.resetGame();
        state.resetGame();
        currentMode = InputMode::MovePawn;
        std::cout << "\nGame reset!\n";
        std::cout << "Current AI difficulty: ";
        switch (aiDifficulty) {
            case Difficulty::Easy: std::cout << "Easy\n"; break;
            case Difficulty::Normal: std::cout << "Normal\n"; break;
            case Difficulty::Hard: std::cout << "Hard\n"; break;
            case Difficulty::Hell: std::cout << "Hell\n"; break;
        }
    }

    // Member variables
    Board board;           ///< Game board state
    State state;           ///< Game state (current player, game status, etc.)
    Rules rules;           ///< Game rules validator
    Render2D renderer;     ///< SFML renderer
    GameView gameView;     ///< Game view for rendering
    
    InputMode currentMode;      ///< Current input mode (move pawn or place wall)
    Difficulty aiDifficulty;    ///< AI difficulty level
    bool aiThinking;            ///< Flag to prevent multiple AI moves
};

// ============================================================================
// Main Entry Point
// ============================================================================

int main() {
    PVEGame game;
    game.run();
    return 0;
}
