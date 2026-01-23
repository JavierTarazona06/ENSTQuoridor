#pragma once

#include "controller/Scene.hpp"
#include "controller/SceneManager.hpp"
#include "model/State.hpp"
#include "model/AI.hpp"

namespace Quoridor {

class GameOverScene : public Scene {
public:
    GameOverScene(SceneManager& manager, GameStatus result, GameMode mode = GameMode::HumanVsHuman, Difficulty difficulty = Difficulty::Normal);
    ~GameOverScene() override = default;

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;

private:
    SceneManager& manager;
    Render2D& renderer;
    GameStatus result;
    GameMode gameMode;
    Difficulty difficulty;
};

} // namespace Quoridor
