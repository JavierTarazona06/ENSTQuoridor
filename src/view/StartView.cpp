#include "view/StartView.hpp"

namespace Quoridor {

void StartView::render(Render2D& render, const Board& board) {
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
}

} // namespace Quoridor
