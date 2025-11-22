#include "view/Render2D.hpp"

namespace Quoridor {

Render2D::Render2D() 
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "ENSTQuoridor") {
    // Optional: Set frame rate limit for smoother rendering
    window.setFramerateLimit(60);
}

Render2D::~Render2D() {
    if (window.isOpen()) {
        window.close();
    }
}

bool Render2D::isOpen() const {
    return window.isOpen();
}

void Render2D::close() {
    window.close();
}

void Render2D::clear() {
    window.clear(sf::Color::Black);
}

void Render2D::display() {
    window.display();
}

// Optional is a result that may exist or not
std::optional<sf::Event> Render2D::pollEvent() {
    return window.pollEvent();
}

void Render2D::drawGrid() {
    // Draw 9x9 grid of cells with borders
    for (unsigned int row = 0; row < GRID_SIZE; ++row) {
        for (unsigned int col = 0; col < GRID_SIZE; ++col) {
            sf::RectangleShape cell({CELL_SIZE, CELL_SIZE});
            cell.setPosition({GRID_OFFSET_X + col * CELL_SIZE, 
                            GRID_OFFSET_Y + row * CELL_SIZE});
            cell.setFillColor(sf::Color(240, 217, 181)); // Beige/tan color for board
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(1.0f);
            
            window.draw(cell);
        }
    }
}

void Render2D::drawPawn(const Board& board, int playerNumber) {
    // Get pawn position from board
    Position pawnPos = board.getPawnPosition(playerNumber);
    
    // Get pawn color from board
    Color pawnColor = board.getPawnColor(playerNumber);
    
    // Calculate screen position (center of cell)
    float screenX = GRID_OFFSET_X + pawnPos.x * CELL_SIZE + CELL_SIZE / 2.0f;
    float screenY = GRID_OFFSET_Y + pawnPos.y * CELL_SIZE + CELL_SIZE / 2.0f;
    
    // Create circle for pawn
    sf::CircleShape pawn(PAWN_RADIUS);
    pawn.setOrigin({PAWN_RADIUS, PAWN_RADIUS}); // Center the circle
    pawn.setPosition({screenX, screenY});
    
    // Set color from board model
    pawn.setFillColor(sf::Color(pawnColor.r, pawnColor.g, pawnColor.b));
    pawn.setOutlineColor(sf::Color::Black);
    pawn.setOutlineThickness(2.0f);
    
    window.draw(pawn);
}

sf::RenderWindow& Render2D::getWindow() {
    return window;
}

} // namespace Quoridor
