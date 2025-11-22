#pragma once

#include "view/Render2D.hpp"
#include "model/Board.hpp"

namespace Quoridor {

// Abstract base class for all views
class View {
public:
    virtual ~View() = default;
    
    // Pure virtual method that all views must implement
    virtual void render(Render2D& render, const Board& board) = 0;
};

} // namespace Quoridor
