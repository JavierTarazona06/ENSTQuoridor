#include "controller/Game.hpp"

namespace Quoridor {

Game::Game()
    : board(),
      state(board),
      rules(),
      renderer(),
      gameView(),
      inputHandler(board, state, rules)
{
    // Initialize default state if needed, but constructors should handle it
}

void Game::run() {
    while (renderer.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    auto event = renderer.pollEvent();
    while (event.has_value()) {
        if (event->is<sf::Event::Closed>()) {
            renderer.close();
        } else {
            inputHandler.handleInput(*event, renderer.getWindow());
        }
        event = renderer.pollEvent();
    }
}

void Game::update() {
    // Game logic updates can go here
    // Currently, most logic is driven by input
}

void Game::render() {
    renderer.clear();
    gameView.render(renderer, board, state);
    renderer.display();
}

} // namespace Quoridor
