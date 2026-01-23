#include "controller/SceneManager.hpp"

namespace Quoridor {

SceneManager::SceneManager(Render2D& renderer)
    : renderer(renderer) {}

void SceneManager::setScene(std::unique_ptr<Scene> scene, GameState state) {
    currentState = state;
    activeScene = std::move(scene);
}

void SceneManager::handleEvent(const sf::Event& event) {
    if (activeScene) {
        activeScene->handleEvent(event);
    }
}

void SceneManager::update(float deltaTime) {
    if (activeScene) {
        activeScene->update(deltaTime);
    }
}

void SceneManager::render() {
    if (activeScene) {
        activeScene->render();
    }
}

} // namespace Quoridor
