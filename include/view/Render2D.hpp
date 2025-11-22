#pragma once

#include <SFML/Graphics.hpp>

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

    // Get window reference
    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow window;
    static constexpr unsigned int WINDOW_WIDTH = 900;
    static constexpr unsigned int WINDOW_HEIGHT = 900;
    static constexpr unsigned int GRID_SIZE = 9;
    static constexpr float CELL_SIZE = 60.0f;
    static constexpr float GRID_OFFSET_X = 180.0f;
    static constexpr float GRID_OFFSET_Y = 180.0f;
};

} // namespace Quoridor
