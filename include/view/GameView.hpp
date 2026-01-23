#pragma once

#include "view/View.hpp"
#include "model/AI.hpp"

namespace Quoridor {

class GameView : public View {
public:
    // Override the render method from View base class
    void render(Render2D& render, const Board& board, const State& state, GameMode mode = GameMode::HumanVsHuman, Difficulty difficulty = Difficulty::Normal) override;

private:
    // Draw game mode and difficulty info box
    void drawGameModeInfoBox(Render2D& render, GameMode mode, Difficulty difficulty);
};

} // namespace Quoridor
