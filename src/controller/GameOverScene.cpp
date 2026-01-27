#include "controller/GameOverScene.hpp"
#include "controller/GameScene.hpp"
#include "controller/MenuScene.hpp"
#include <string>

namespace Quoridor {

GameOverScene::GameOverScene(SceneManager& manager, GameStatus result, const GameConfig& gameConfig)
    : manager(manager), renderer(manager.getRenderer()), result(result), config(gameConfig) {
    std::string message = (result == GameStatus::Player1Won) ? "Red Player Wins!" : "Blue Player Wins!";
    renderer.showMessage(message + " Press Enter to restart or Esc for Menu", {255,255,255}, -1.0f);
}

void GameOverScene::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        renderer.close();
        return;
    }

    if (event.is<sf::Event::KeyPressed>()) {
        const auto key = event.getIf<sf::Event::KeyPressed>()->code;
        if (key == sf::Keyboard::Key::Enter) {
            // Restart with the same game configuration
            manager.setScene(std::make_unique<GameScene>(manager, config), GameState::Playing);
            return;
        }
        if (key == sf::Keyboard::Key::Escape) {
            renderer.showMessage("", {0,0,0}, 0.0f); // Clear message
            manager.setScene(std::make_unique<MenuScene>(manager), GameState::Menu);
            return;
        }
    }
}

void GameOverScene::update(float deltaTime) {
    renderer.updateMessage(deltaTime);
}

void GameOverScene::render() {
    renderer.clear();

    const float centerX = Render2D::getWindowWidth() / 2.0f;
    const float centerY = Render2D::getWindowHeight() / 2.0f;

    std::string title = (result == GameStatus::Player1Won) ? "Red Player Wins" : "Blue Player Wins";
    renderer.drawText(title, centerX, centerY - 40.0f, 40, sf::Color::White, 1);
    renderer.drawText("Enter: Restart", centerX, centerY + 10.0f, 24, sf::Color(200, 255, 200), 3);
    renderer.drawText("Esc: Main Menu", centerX, centerY + 50.0f, 22, sf::Color(220, 220, 220), 3);

    renderer.display();
}

} // namespace Quoridor
