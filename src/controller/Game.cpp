#include "controller/Game.hpp"

namespace Quoridor {

Game::Game()
    : renderer(), sceneManager(renderer) {
    sceneManager.setScene(std::make_unique<MenuScene>(sceneManager), GameState::Menu);
}

void Game::run() {
    sf::Clock clock;

    while (renderer.isOpen()) {
        const float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    auto event = renderer.pollEvent();
    while (event.has_value()) {
        sceneManager.handleEvent(*event);
        event = renderer.pollEvent();
    }
}

void Game::update(float deltaTime) {
    sceneManager.update(deltaTime);
}

void Game::render() {
    sceneManager.render();
}

} // namespace Quoridor
