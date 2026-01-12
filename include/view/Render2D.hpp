#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <optional>
#include "model/Board.hpp"
#include "model/State.hpp"

namespace Quoridor {

class Render2D {
public:
    Render2D();
    ~Render2D();

    // Window management
    bool isOpen() const;
    void close();
    void clear();
    void display();

    // Event handling
    std::optional<sf::Event> pollEvent();

    // Rendering methods
    void drawGrid();
    void drawPawn(const Board& board, int playerNumber);
    void drawText(const std::string& text, float x, float y, unsigned int fontSize, const sf::Color& color, int fontType = 3);
    void drawWalls(const Board& board);
    void drawWallPreview(const std::optional<Wall>& previewWall);
    void drawHUD(const Board& board, const State& state);

    // Font management
    bool loadFont(const std::string& fontPath, int fontType);

    // Get window reference
    sf::RenderWindow& getWindow();

    // Getters for constants
    static constexpr unsigned int getWindowWidth() { return WINDOW_WIDTH; }
    static constexpr unsigned int getWindowHeight() { return WINDOW_HEIGHT; }
    static constexpr float getGridOffsetX() { return GRID_OFFSET_X; }
    static constexpr float getGridOffsetY() { return GRID_OFFSET_Y; }
    static constexpr float getCellSize() { return CELL_SIZE; }

private:
    sf::RenderWindow window;
    sf::Font fontTitle1;
    sf::Font fontTitle2;
    sf::Font fontTitle3;
    sf::Font fontText;
    static constexpr unsigned int WINDOW_WIDTH = 900;
    static constexpr unsigned int WINDOW_HEIGHT = 900;
    static constexpr float CELL_SIZE = 60.0f;
    static constexpr float GRID_OFFSET_X = 180.0f;
    static constexpr float GRID_OFFSET_Y = 180.0f;
    static constexpr float PAWN_RADIUS = 20.0f;
    static constexpr sf::Color BOARD_COLOR = sf::Color(240, 217, 181); //Beige

    // Wall constants
    static constexpr float WALL_THICKNESS = 6.0f;  // Wall thickness (~3px on each side)
    static constexpr sf::Color WALL_COLOR = sf::Color(139, 69, 19); // Brown (SaddleBrown)
    static constexpr sf::Color WALL_OUTLINE_COLOR = sf::Color(80, 40, 10); // Darker outline
    static constexpr sf::Color WALL_PREVIEW_COLOR = sf::Color(139, 69, 19, 100); // Semi-transparent preview
    
    // HUD constants
    static constexpr float HUD_BOX_WIDTH = 160.0f;
    static constexpr float HUD_BOX_HEIGHT = 220.0f;
    static constexpr sf::Color HUD_BACKGROUND_COLOR = sf::Color(40, 40, 40); // Dark gray
    static constexpr sf::Color HUD_BORDER_COLOR = sf::Color::White;
    static constexpr float HUD_BORDER_THICKNESS = 2.0f;
};

} // namespace Quoridor
