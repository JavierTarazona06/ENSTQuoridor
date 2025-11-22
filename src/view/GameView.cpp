#include "view/GameView.hpp"
#include <string>

namespace Quoridor {

void GameView::render(Render2D& render, const Board& board, const State& state) {
    // Draw title text
    float centerX = Render2D::getWindowWidth() * 0.5f;
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
    
    // Draw info box
    float boxWidth = 120.0f;
    float boxHeight = 200.0f;
    float boxX = (Render2D::getGridOffsetX() - boxWidth) * 0.5f;
    float boxY = (Render2D::getCellSize() * BOARD_SIZE * 0.5f) + Render2D::getGridOffsetY() - boxHeight * 0.5f;
    
    sf::RectangleShape infoBox({boxWidth, boxHeight});
    infoBox.setPosition({boxX, boxY});
    infoBox.setFillColor(sf::Color(0, 100, 0)); // Dark green
    infoBox.setOutlineColor(sf::Color::White);
    infoBox.setOutlineThickness(3.0f);
    render.getWindow().draw(infoBox);
    
    // Section 1: Player info
    float section1Y = boxY + 20.0f;
    render.drawText("Joueur", boxX + boxWidth * 0.5f, section1Y, 20, sf::Color::White, 1);
    
    std::string playerText = std::to_string(state.getCurrentPlayer() + 1);
    render.drawText(playerText, boxX + boxWidth * 0.5f, section1Y + 35.0f, 28, sf::Color::Yellow, 2);
    
    // Section 2: Status info
    float section2Y = boxY + 110.0f;
    render.drawText("Status", boxX + boxWidth * 0.5f, section2Y, 20, sf::Color::White, 1);
    
    std::string statusText;
    switch (state.getGameStatus()) {
        case GameStatus::Playing:
            statusText = "Playing";
            break;
        case GameStatus::Player1Won:
            statusText = "P1 Won";
            break;
        case GameStatus::Player2Won:
            statusText = "P2 Won";
            break;
    }
    render.drawText(statusText, boxX + boxWidth * 0.5f, section2Y + 35.0f, 18, sf::Color::White, 3);
}

} // namespace Quoridor
