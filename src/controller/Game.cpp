#include "controller/Game.hpp"

namespace Quoridor {

Game::Game()
    : board(),
      state(),
      rules(),
      renderer(),
      gameView(),
      inputHandler(board, state, rules, renderer)
{
    // Initialize default state if needed, but constructors should handle it
}

void Game::run() {
    sf::Clock clock;
    
    // Display initial player turn message
    int currentPlayer = state.getCurrentPlayer();
    Color playerColor = board.getPawnColor(currentPlayer);
    std::string playerName = "Player " + std::to_string(currentPlayer + 1);
    renderer.showMessage(playerName + " Turn, select pawn to start moving or press w to place wall", {255,255,255}, -1.0f);
    
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
        if (event->is<sf::Event::Closed>()) {
            renderer.close();
        } else {
            inputHandler.handleInput(*event, renderer.getWindow());
        }
        event = renderer.pollEvent();
    }
}

void Game::update(float deltaTime) {
    // Update message box timer
    renderer.updateMessage(deltaTime);
    
    // Game logic updates can go here
    // Currently, most logic is driven by input
}

void Game::render() {
    renderer.clear();
    gameView.render(renderer, board, state);
    renderer.display();
}

} // namespace Quoridor
