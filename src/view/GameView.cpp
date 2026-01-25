#include "view/GameView.hpp"
#include "model/Rules.hpp"
#include <string>
#include <optional>

namespace Quoridor {

void GameView::render(Render2D& render, const Board& board, const State& state, GameMode mode, Difficulty difficulty) {
    // Draw title text
    float centerX = Render2D::getWindowWidth() * 0.5f;
    float centerY = Render2D::getWindowHeight() * 0.5f;
    float titleY = Render2D::getGridOffsetY() * 0.38f;
    render.drawText("ENSTQuoridor", centerX, titleY, 48, sf::Color::White, 0);

    // Draw developers text
    float authorsY = Render2D::getGridOffsetY() * 0.75f;
    render.drawText("Developed by Javier Tarazona & Liang Tianyi", centerX, authorsY, 18, sf::Color::White, 1);

    // Draw game elements
    render.drawGrid();
    
    // Draw all players' pawns
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        render.drawPawn(board, i);
    }
    
    // Draw placed walls
    render.drawWalls(board);
    
    // Draw wall preview (if in wall placement mode)
    auto previewWall = state.getPreviewWall();
    if (previewWall.has_value()) {
        render.drawWallPreview(previewWall);
    }

    // Draw selection and valid moves
    std::optional<Position> selectedPawn = state.getSelectedPawn();
    if (selectedPawn.has_value()) {
        Position p = selectedPawn.value();
        
        // Highlight selected pawn (e.g., draw a circle or box around it)
        // Calculate screen position
        float cellSize = Render2D::getCellSize();
        float screenX = Render2D::getGridOffsetX() + p.x * cellSize;
        float screenY = Render2D::getGridOffsetY() + p.y * cellSize;
        
        sf::RectangleShape highlight({cellSize, cellSize});
        highlight.setPosition({screenX, screenY});
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        render.getWindow().draw(highlight);
        
        // Show valid moves
        int currentPlayer = state.getCurrentPlayer();
        
        // Iterate over all cells to find valid moves (optimization: just check neighbors)
        // But checking neighbors is complex due to jumps. 
        // Rules::isValidMove handles it.
        // We can check a reasonable range around the pawn (e.g. +/- 2)
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                if (Rules::isValidMove(board, currentPlayer, p.y, p.x, r, c)) {
                    // Draw a small dot in the center of the valid target cell
                    float targetX = Render2D::getGridOffsetX() + c * cellSize + cellSize / 2.0f;
                    float targetY = Render2D::getGridOffsetY() + r * cellSize + cellSize / 2.0f;
                    
                    sf::CircleShape dot(5.0f);
                    dot.setOrigin({5.0f, 5.0f});
                    dot.setPosition({targetX, targetY});
                    dot.setFillColor(sf::Color(100, 250, 100, 200)); // Semi-transparent green
                    render.getWindow().draw(dot);
                }
            }
        }
    }
    
    // Draw HUD (Player info, walls remaining, status)
    render.drawHUD(board, state);

    // Draw message box
    render.drawMessage();

    // Draw game mode and difficulty info box
    drawGameModeInfoBox(render, mode, difficulty);

    // Draw Victory Overlay if game is over
    if (state.getGameStatus() != GameStatus::Playing) {
        // Semi-transparent overlay
        sf::RectangleShape overlay({(float)Render2D::getWindowWidth(), (float)Render2D::getWindowHeight()});
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        render.getWindow().draw(overlay);

        // Victory Text
        std::string victoryText;
        sf::Color victoryColor;
        
        if (state.getGameStatus() == GameStatus::Player1Won) {
            victoryText = "PLAYER 1 WINS!";
            victoryColor = sf::Color::Red;
        } else {
            victoryText = "PLAYER 2 WINS!";
            victoryColor = sf::Color::Blue;
        }

        float windowCenterX = Render2D::getWindowWidth() * 0.5f;
        float windowCenterY = Render2D::getWindowHeight() * 0.5f;

        // Draw shadow
        render.drawText(victoryText, windowCenterX + 4.0f, windowCenterY + 4.0f, 60, sf::Color::Black, 0);
        // Draw text
        render.drawText(victoryText, windowCenterX, windowCenterY, 60, victoryColor, 0);
        
        render.drawText("Press R to restart", windowCenterX, windowCenterY + 80.0f, 30, sf::Color::White, 1);
    }
}

void GameView::drawGameModeInfoBox(Render2D& render, GameMode mode, Difficulty difficulty) {
    // Draw game mode and difficulty info box (right side, middle of grid)
    float gridRightX = Render2D::getGridOffsetX() + BOARD_SIZE * Render2D::getCellSize() + 20.0f;
    float gridMiddleY = Render2D::getGridOffsetY() + (BOARD_SIZE * Render2D::getCellSize()) / 2.0f;
    
    // Draw info box background
    float boxWidth = 200.0f;
    float boxHeight = (mode == GameMode::HumanVsAI) ? 120.0f : 60.0f;
    sf::RectangleShape infoBox({boxWidth, boxHeight});
    infoBox.setPosition({gridRightX, gridMiddleY - boxHeight / 2.0f});
    infoBox.setFillColor(sf::Color(30, 30, 30, 200));
    infoBox.setOutlineColor(sf::Color(255, 255, 255));
    infoBox.setOutlineThickness(2.0f);
    render.getWindow().draw(infoBox);
    
    // Draw mode text (center vertically for PvP, top offset for PvE to leave room for difficulty line)
    std::string modeText = (mode == GameMode::HumanVsAI) ? "AI vs Human" : "Human vs Human";
    float modeTextY = (mode == GameMode::HumanVsAI) ? (gridMiddleY - 30.0f) : gridMiddleY;
    render.drawText(modeText, gridRightX + boxWidth / 2.0f, modeTextY, 18, sf::Color(100, 200, 255), 0);
    
    // Draw difficulty if AI mode
    if (mode == GameMode::HumanVsAI) {
        std::string diffText;
        sf::Color diffColor = sf::Color::White;
        
        if (difficulty == Difficulty::Easy) {
            diffText = "Easy";
            diffColor = sf::Color(100, 255, 100);
        } else if (difficulty == Difficulty::Normal) {
            diffText = "Normal";
            diffColor = sf::Color(255, 255, 100);
        } else if (difficulty == Difficulty::Hard) {
            diffText = "Hard";
            diffColor = sf::Color(255, 165, 0);
        } else if (difficulty == Difficulty::Hell) {
            diffText = "Hell";
            diffColor = sf::Color(255, 50, 50);
        }
        
        render.drawText("Difficulty: " + diffText, gridRightX + boxWidth / 2.0f, gridMiddleY + 2.0f, 16, diffColor, 0);
        render.drawText("Press 1-4 to change", gridRightX + boxWidth / 2.0f, gridMiddleY + 30.0f, 16, sf::Color(180, 180, 180), 0);
    }
}

} // namespace Quoridor
