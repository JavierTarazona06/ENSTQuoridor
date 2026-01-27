#pragma once

#include <memory>
#include <SFML/Window/Event.hpp>
#include "controller/Scene.hpp"
#include "view/Render2D.hpp"

namespace Quoridor {

enum class GameState {
    Menu,
    Playing,
    GameOver,
    HowToPlay
};

class SceneManager {
public:
    explicit SceneManager(Render2D& renderer);

    void setScene(std::unique_ptr<Scene> scene, GameState state);

    void handleEvent(const sf::Event& event);
    void update(float deltaTime);
    void render();

    Render2D& getRenderer() const { return renderer; }
    GameState getState() const { return currentState; }

private:
    Render2D& renderer;
    std::unique_ptr<Scene> activeScene;
    GameState currentState { GameState::Menu };
};

} // namespace Quoridor
