#pragma once

#include "controller/Scene.hpp"
#include "controller/SceneManager.hpp"
#include "model/State.hpp"
#include "model/GameConfig.hpp"

namespace Quoridor {

class GameOverScene : public Scene {
public:
    GameOverScene(SceneManager& manager, GameStatus result, const GameConfig& config = GameConfig());
    ~GameOverScene() override = default;

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;

private:
    SceneManager& manager;
    Render2D& renderer;
    GameStatus result;
    GameConfig config;
};

} // namespace Quoridor
