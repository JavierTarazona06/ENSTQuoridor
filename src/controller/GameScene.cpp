#include "controller/GameScene.hpp"
#include "controller/GameOverScene.hpp"
#include "controller/MenuScene.hpp"
#include <string>

namespace Quoridor {

GameScene::GameScene(SceneManager& manager)
    : manager(manager),
      renderer(manager.getRenderer()),
      board(),
      state(),
      rules(),
      gameView(),
      inputHandler(board, state, rules, renderer) {
    int currentPlayer = state.getCurrentPlayer();
    std::string playerName = "Player " + std::to_string(currentPlayer + 1);
    renderer.showMessage(playerName + " Turn, select pawn to start moving or press w to place wall", {255,255,255}, -1.0f);
}

void GameScene::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        renderer.close();
        return;
    }

    if (event.is<sf::Event::KeyPressed>()) {
        const auto key = event.getIf<sf::Event::KeyPressed>()->code;
        if (key == sf::Keyboard::Key::Escape) {
            manager.setScene(std::make_unique<MenuScene>(manager), GameState::Menu);
            return;
        }
    }

    inputHandler.handleInput(event, renderer.getWindow());
}

void GameScene::update(float deltaTime) {
    renderer.updateMessage(deltaTime);
    checkForGameOver();
}

void GameScene::render() {
    renderer.clear();
    gameView.render(renderer, board, state);
    renderer.display();
}

void GameScene::checkForGameOver() {
    if (state.getGameStatus() != GameStatus::Playing) {
        manager.setScene(std::make_unique<GameOverScene>(manager, state.getGameStatus()), GameState::GameOver);
    }
}

} // namespace Quoridor
