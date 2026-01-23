#pragma once

#include "view/Render2D.hpp"
#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/AI.hpp"

namespace Quoridor {

// Abstract base class for all views
class View {
public:
    virtual ~View() = default;
    
    // Pure virtual method that all views must implement
    virtual void render(Render2D& render, const Board& board, const State& state, GameMode mode = GameMode::HumanVsHuman, Difficulty difficulty = Difficulty::Normal) = 0;
};

} // namespace Quoridor
