#pragma once

#include "controller/Scene.hpp"
#include "controller/SceneManager.hpp"
#include "controller/InputHandler.hpp"
#include "view/GameView.hpp"
#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"

namespace Quoridor {

class GameScene : public Scene {
public:
    explicit GameScene(SceneManager& manager);
    ~GameScene() override = default;

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;

private:
    void checkForGameOver();

    SceneManager& manager;
    Render2D& renderer;
    Board board;
    State state;
    Rules rules;
    GameView gameView;
    InputHandler inputHandler;
};

} // namespace Quoridor
