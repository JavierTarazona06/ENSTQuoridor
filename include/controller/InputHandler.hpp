#pragma once

#include <SFML/Graphics.hpp>
#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"
#include "view/Render2D.hpp"

namespace Quoridor {

enum class InputMode {
    MovePawn,
    PlaceWall
};

class InputHandler {
public:
    InputHandler(Board& board, State& state, Rules& rules);
    ~InputHandler() = default;

    void handleInput(const sf::Event& event, const sf::RenderWindow& window);
    
    InputMode getInputMode() const { return currentMode; }
    void setInputMode(InputMode mode) { currentMode = mode; }

private:
    // Helper to convert pixel coordinates to grid coordinates
    // Returns true if the click was within the grid
    bool pixelToGrid(int pixelX, int pixelY, int& row, int& col) const;

    // Handle mouse click events
    void handleMouseClick(int pixelX, int pixelY);
    
    // Handle mouse move for wall preview
    void handleMouseMove(int pixelX, int pixelY);

    Board& board;
    State& state;
    Rules& rules;
    
    InputMode currentMode = InputMode::MovePawn;
};

} // namespace Quoridor
