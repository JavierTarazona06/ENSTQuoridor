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

sf::RenderWindow& Render2D::getWindow() {
    return window;
}

} // namespace Quoridor
