#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <optional>
#include "model/Board.hpp"

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

    static constexpr float WALL_THICKNESS = 6.0f;  // Wall thickness (~3px on each side)
    static constexpr sf::Color WALL_COLOR = sf::Color(139, 69, 19); // Brown (SaddleBrown)
    static constexpr sf::Color WALL_OUTLINE_COLOR = sf::Color(80, 40, 10); // Darker outline
};

} // namespace Quoridor
