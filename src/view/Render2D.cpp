#include "view/Render2D.hpp"
#include <iostream>

namespace Quoridor {

Render2D::Render2D() 
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "ENSTQuoridor") {
    // Optional: Set frame rate limit for smoother rendering
    window.setFramerateLimit(60);
    
    // Load all fonts
    loadFont(std::string(FONT_DIR) + "/arial/ArialCEMTBlack.ttf", 0); // title1
    loadFont(std::string(FONT_DIR) + "/arial/ArialCEBoldItalic.ttf", 1); // title2
    loadFont(std::string(FONT_DIR) + "/arial/ARIALBD.TTF", 2); // title3
    loadFont(std::string(FONT_DIR) + "/arial/ARIAL.TTF", 3); // text
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
    for (unsigned int row = 0; row < BOARD_SIZE; ++row) {
        for (unsigned int col = 0; col < BOARD_SIZE; ++col) {
            sf::RectangleShape cell({CELL_SIZE, CELL_SIZE});
            cell.setPosition({GRID_OFFSET_X + col * CELL_SIZE, 
                            GRID_OFFSET_Y + row * CELL_SIZE});
            cell.setFillColor(BOARD_COLOR);
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

bool Render2D::loadFont(const std::string& fontPath, int fontType) {
    sf::Font* targetFont = nullptr;
    
    // Select which font to load based on fontType
    switch (fontType) {
        case 0: targetFont = &fontTitle1; break;
        case 1: targetFont = &fontTitle2; break;
        case 2: targetFont = &fontTitle3; break;
        case 3: targetFont = &fontText; break;
        default:
            std::cerr << "Error: Invalid font type " << fontType << std::endl;
            return false;
    }
    
    if (!targetFont->openFromFile(fontPath)) {
        std::cerr << "Error: Could not load font from " << fontPath << std::endl;
        return false;
    }
    return true;
}

void Render2D::drawText(const std::string& text, float x, float y, unsigned int fontSize, const sf::Color& color, int fontType) {
    // Select font based on fontType
    sf::Font* selectedFont = nullptr;
    switch (fontType) {
        case 0: selectedFont = &fontTitle1; break;
        case 1: selectedFont = &fontTitle2; break;
        case 2: selectedFont = &fontTitle3; break;
        case 3: selectedFont = &fontText; break;
        default: selectedFont = &fontText; break; // Default to text font
    }
    
    sf::Text sfText(*selectedFont);
    sfText.setString(text);
    sfText.setCharacterSize(fontSize);
    sfText.setFillColor(color);
    
    // Center the text at the specified position
    auto bounds = sfText.getLocalBounds();
    sfText.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    sfText.setPosition({x, y});
    
    window.draw(sfText);
}

sf::RenderWindow& Render2D::getWindow() {
    return window;
}

} // namespace Quoridor
