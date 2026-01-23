#pragma once

#include "view/Render2D.hpp"
#include "controller/SceneManager.hpp"
#include "controller/MenuScene.hpp"

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

    Render2D renderer;
    SceneManager sceneManager;
};

} // namespace Quoridor
