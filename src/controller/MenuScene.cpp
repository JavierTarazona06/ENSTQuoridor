#include "controller/MenuScene.hpp"
#include "controller/GameScene.hpp"

namespace Quoridor {

MenuScene::MenuScene(SceneManager& manager)
    : manager(manager), renderer(manager.getRenderer()) {}

void MenuScene::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        renderer.close();
        return;
    }

    if (event.is<sf::Event::KeyPressed>()) {
        const auto key = event.getIf<sf::Event::KeyPressed>()->code;
        if (key == sf::Keyboard::Key::Enter) {
            manager.setScene(std::make_unique<GameScene>(manager), GameState::Playing);
            return;
        }
        if (key == sf::Keyboard::Key::Escape) {
            renderer.close();
            return;
        }
    }
}

void MenuScene::update(float deltaTime) {
    renderer.updateMessage(deltaTime);
}

void MenuScene::render() {
    renderer.clear();

    // Optional logo
    renderer.drawLogo();

    const float centerX = Render2D::getWindowWidth() / 2.0f;
    const float centerY = Render2D::getWindowHeight() / 2.0f;

    renderer.drawText("ENSTQuoridor", centerX, centerY - 140.0f, 48, sf::Color::White, 1);
    renderer.drawText("Press Enter to Play", centerX, centerY - 20.0f, 28, sf::Color(200, 255, 200), 3);
    renderer.drawText("Press Esc to Quit", centerX, centerY + 30.0f, 24, sf::Color(220, 220, 220), 3);

    renderer.display();
}

} // namespace Quoridor
