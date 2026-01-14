#pragma once

#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"
#include "view/Render2D.hpp"
#include "view/GameView.hpp"
#include "controller/InputHandler.hpp"

namespace Quoridor {

class Game {
public:
    Game();
    ~Game() = default;

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    Board board;
    State state;
    Rules rules;
    Render2D renderer;
    GameView gameView;
    InputHandler inputHandler;
};

} // namespace Quoridor
