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

void Render2D::drawWalls(const Board& board) {
    
    const std::vector<Wall>& walls = board.getWalls();
    
    for (const Wall& wall : walls) {
        sf::RectangleShape wallShape;
        
        if (wall.orientation == Orientation::Horizontal) {
            // Horizontal wall: width = 2 * CELL_SIZE (covers 2 cells), height = WALL_THICKNESS
            // Position: between rows, centered on the bottom edge of the cell
            wallShape.setSize({2 * CELL_SIZE, WALL_THICKNESS});
            
            // Wall is positioned at the bottom edge of the cell (wall.pos.y, wall.pos.x)
            // and extends 2 cells horizontally
            float screenX = GRID_OFFSET_X + wall.pos.x * CELL_SIZE;
            float screenY = GRID_OFFSET_Y + (wall.pos.y + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f;
            
            wallShape.setPosition({screenX, screenY});
        } else {
            // Vertical wall: width = WALL_THICKNESS, height = 2 * CELL_SIZE (covers 2 cells)
            // Position: between columns, centered on the right edge of the cell
            wallShape.setSize({WALL_THICKNESS, 2 * CELL_SIZE});
            
            // Wall is positioned at the right edge of the cell (wall.pos.y, wall.pos.x)
            // and extends 2 cells vertically
            float screenX = GRID_OFFSET_X + (wall.pos.x + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f;
            float screenY = GRID_OFFSET_Y + wall.pos.y * CELL_SIZE;
            
            wallShape.setPosition({screenX, screenY});
        }
        
        wallShape.setFillColor(WALL_COLOR);
        wallShape.setOutlineColor(WALL_OUTLINE_COLOR);
        wallShape.setOutlineThickness(1.0f);
        
        window.draw(wallShape);
    }
}

void Render2D::drawWallPreview(const std::optional<Wall>& previewWall) {
    if (!previewWall.has_value()) {
        return;
    }
    
    const Wall& wall = previewWall.value();
    sf::RectangleShape wallShape;
    
    if (wall.orientation == Orientation::Horizontal) {
        wallShape.setSize({2 * CELL_SIZE, WALL_THICKNESS});
        float screenX = GRID_OFFSET_X + wall.pos.x * CELL_SIZE;
        float screenY = GRID_OFFSET_Y + (wall.pos.y + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f;
        wallShape.setPosition({screenX, screenY});
    } else {
        wallShape.setSize({WALL_THICKNESS, 2 * CELL_SIZE});
        float screenX = GRID_OFFSET_X + (wall.pos.x + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f;
        float screenY = GRID_OFFSET_Y + wall.pos.y * CELL_SIZE;
        wallShape.setPosition({screenX, screenY});
    }
    
    // Use semi-transparent version of wall colors from header constants
    sf::Color previewColor = WALL_PREVIEW_COLOR;
    sf::Color previewOutline(WALL_OUTLINE_COLOR.r, WALL_OUTLINE_COLOR.g, WALL_OUTLINE_COLOR.b, 100); // Last is transparency
    
    wallShape.setFillColor(previewColor);
    wallShape.setOutlineColor(previewOutline);
    wallShape.setOutlineThickness(1.0f);
    
    window.draw(wallShape);
}

// TODO: Acomodar mejor el cuadro de HUD y su informacion
void Render2D::drawHUD(const Board& board, const State& state) {
    // HUD position: top-left corner, outside the grid
    float hudX = GRID_OFFSET_X - HUD_BOX_WIDTH - 20.0f;
    float hudY = GRID_OFFSET_Y;
    
    // Draw HUD background box
    sf::RectangleShape hudBox({HUD_BOX_WIDTH, HUD_BOX_HEIGHT});
    hudBox.setPosition({hudX, hudY});
    hudBox.setFillColor(HUD_BACKGROUND_COLOR);
    hudBox.setOutlineColor(HUD_BORDER_COLOR);
    hudBox.setOutlineThickness(HUD_BORDER_THICKNESS);
    window.draw(hudBox);
    
    // Get current player info
    int currentPlayer = state.getCurrentPlayer();
    Color playerColor = board.getPawnColor(currentPlayer);
    sf::Color sfPlayerColor(playerColor.r, playerColor.g, playerColor.b);
    
    // Section 1: Player Title
    float section1Y = hudY + 15.0f;
    drawText("PLAYER", hudX + HUD_BOX_WIDTH * 0.5f, section1Y, 18, sf::Color::White, 2);
    
    // Section 2: Player Number with color indicator
    float section2Y = hudY + 40.0f;
    std::string playerNum = std::to_string(currentPlayer + 1);
    drawText(playerNum, hudX + HUD_BOX_WIDTH * 0.5f, section2Y, 40, sfPlayerColor, 2);
    
    // Section 3: Walls remaining
    float section3Y = hudY + 90.0f;
    drawText("WALLS", hudX + HUD_BOX_WIDTH * 0.5f, section3Y, 14, sf::Color::White, 2);
    
    int wallsRemaining = board.getWallsRemaining(currentPlayer);
    std::string wallsText = std::to_string(wallsRemaining) + "/10";
    
    // Color code walls: Green if plenty, Yellow if medium, Red if few
    sf::Color wallsColor = sf::Color::Green;
    if (wallsRemaining <= 3) wallsColor = sf::Color::Red;
    else if (wallsRemaining <= 5) wallsColor = sf::Color::Yellow;
    
    drawText(wallsText, hudX + HUD_BOX_WIDTH * 0.5f, section3Y + 28.0f, 28, wallsColor, 2);
    
    // Section 4: Game Status
    float section4Y = hudY + 145.0f;
    drawText("STATUS", hudX + HUD_BOX_WIDTH * 0.5f, section4Y, 12, sf::Color::White, 2);
    
    std::string statusText;
    sf::Color statusColor = sf::Color::Green;
    
    switch (state.getGameStatus()) {
        case GameStatus::Playing:
            statusText = "PLAYING";
            statusColor = sf::Color::Green;
            break;
        case GameStatus::Player1Won:
            statusText = "P1 WINS";
            statusColor = sf::Color::Red;
            break;
        case GameStatus::Player2Won:
            statusText = "P2 WINS";
            statusColor = sf::Color::Blue;
            break;
    }
    
    drawText(statusText, hudX + HUD_BOX_WIDTH * 0.5f, section4Y + 22.0f, 16, statusColor, 2);
}

sf::RenderWindow& Render2D::getWindow() {
    return window;
}

} // namespace Quoridor
