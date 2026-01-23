#pragma once

#include "controller/Scene.hpp"
#include "controller/SceneManager.hpp"
#include "model/GameConfig.hpp"

namespace Quoridor {

class MenuScene : public Scene {
public:
    explicit MenuScene(SceneManager& manager);
    ~MenuScene() override = default;

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;

private:
    SceneManager& manager;
    Render2D& renderer;
    GameConfig config;
};

} // namespace Quoridor
