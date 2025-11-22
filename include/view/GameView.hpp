#pragma once

#include "view/View.hpp"

namespace Quoridor {

class GameView : public View {
public:
    // Override the render method from View base class
    void render(Render2D& render, const Board& board, const State& state) override;
};

} // namespace Quoridor
