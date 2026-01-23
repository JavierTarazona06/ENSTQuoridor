#include "controller/MenuScene.hpp"
#include "controller/GameScene.hpp"

namespace Quoridor {

MenuScene::MenuScene(SceneManager& manager)
    : manager(manager), renderer(manager.getRenderer()),
      config() {}

void MenuScene::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        renderer.close();
        return;
    }

    if (event.is<sf::Event::KeyPressed>()) {
        const auto key = event.getIf<sf::Event::KeyPressed>()->code;
        
        // Enter - Start game with selected config
        if (key == sf::Keyboard::Key::Enter) {
            manager.setScene(std::make_unique<GameScene>(manager, config), GameState::Playing);
            return;
        }
        
        // Escape - Quit
        if (key == sf::Keyboard::Key::Escape) {
            renderer.close();
            return;
        }
        
        // H - Human vs Human
        if (key == sf::Keyboard::Key::H) {
            config.mode = GameMode::HumanVsHuman;
            renderer.showMessage("Mode: Human vs Human", {100, 255, 100}, 1.5f);
            return;
        }
        
        // A - Human vs AI
        if (key == sf::Keyboard::Key::A) {
            config.mode = GameMode::HumanVsAI;
            renderer.showMessage("Mode: Human vs AI", {255, 200, 100}, 1.5f);
            return;
        }
        
        // 1-4 - AI Difficulty (only if AI mode selected)
        if (config.isAIMode()) {
            auto scancode = event.getIf<sf::Event::KeyPressed>()->scancode;
            if (key == sf::Keyboard::Key::Num1 || key == sf::Keyboard::Key::Numpad1 || scancode == sf::Keyboard::Scancode::Num1) {
                config.difficulty = Difficulty::Easy;
                renderer.showMessage("AI Difficulty: Easy", {100, 255, 100}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num2 || key == sf::Keyboard::Key::Numpad2 || scancode == sf::Keyboard::Scancode::Num2) {
                config.difficulty = Difficulty::Normal;
                renderer.showMessage("AI Difficulty: Normal", {255, 255, 100}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num3 || key == sf::Keyboard::Key::Numpad3 || scancode == sf::Keyboard::Scancode::Num3) {
                config.difficulty = Difficulty::Hard;
                renderer.showMessage("AI Difficulty: Hard", {255, 165, 0}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num4 || key == sf::Keyboard::Key::Numpad4 || scancode == sf::Keyboard::Scancode::Num4) {
                config.difficulty = Difficulty::Hell;
                renderer.showMessage("AI Difficulty: Hell", {255, 50, 50}, 1.5f);
                return;
            }
        }
    }

    // Mouse click on mode labels acts like pressing H or A
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mouse = event.getIf<sf::Event::MouseButtonPressed>()->position;
        const float centerX = Render2D::getWindowWidth() / 2.0f;
        const float centerY = Render2D::getWindowHeight() / 2.0f;

        // Define simple bounding boxes around the text labels
        const float boxHalfWidth = 160.0f;
        const float hvhTop = centerY - 65.0f;   // a bit above the text baseline
        const float hvhBottom = centerY - 35.0f;
        const float hvaiTop = centerY - 30.0f;
        const float hvaiBottom = centerY + 0.0f;

        const bool overHVH = (mouse.x >= centerX - boxHalfWidth && mouse.x <= centerX + boxHalfWidth &&
                              mouse.y >= hvhTop && mouse.y <= hvhBottom);
        const bool overHVAI = (mouse.x >= centerX - boxHalfWidth && mouse.x <= centerX + boxHalfWidth &&
                               mouse.y >= hvaiTop && mouse.y <= hvaiBottom);

        if (overHVH) {
            config.mode = GameMode::HumanVsHuman;
            renderer.showMessage("Mode: Human vs Human", {100, 255, 100}, 1.5f);
            return;
        }

        if (overHVAI) {
            config.mode = GameMode::HumanVsAI;
            renderer.showMessage("Mode: Human vs AI", {255, 200, 100}, 1.5f);
            return;
        }
    }
}

void MenuScene::update(float deltaTime) {
    renderer.updateMessage(deltaTime);
}

void MenuScene::render() {
    renderer.clear();

    const float centerX = Render2D::getWindowWidth() / 2.0f;
    const float centerY = Render2D::getWindowHeight() / 2.0f;

    // Title
    renderer.drawText("ENSTQuoridor", centerX, centerY - 250.0f, 48, sf::Color::White, 1);
    renderer.drawText("Developed by Javier Tarazona & Liang Tianyi", centerX, centerY - 210.0f, 18, sf::Color::White, 1);
    
    // Game Mode Selection
    renderer.drawText("Select Game Mode:", centerX, centerY - 150.0f, 24, sf::Color(200, 200, 200), 2);
    renderer.drawText("(Type letter or number)", centerX, centerY - 100.0f, 24, sf::Color(200, 200, 200), 2);
    
    sf::Color hvhColor = config.isPvPMode() ? sf::Color(100, 255, 100) : sf::Color(150, 150, 150);
    sf::Color hvaiColor = config.isAIMode() ? sf::Color(255, 200, 100) : sf::Color(150, 150, 150);
    
    renderer.drawText("[H] Human vs Human", centerX, centerY - 50.0f, 22, hvhColor, 3);
    renderer.drawText("[A] Human vs AI", centerX, centerY - 15.0f, 22, hvaiColor, 3);
    
    // Difficulty selection (only show if AI mode)
    if (config.isAIMode()) {
        renderer.drawText("AI Difficulty:", centerX, centerY + 30.0f, 22, sf::Color(200, 200, 200), 2);
        
        std::string diffText;
        sf::Color diffColor;
        switch(config.difficulty) {
            case Difficulty::Easy:   diffText = "[1] Easy"; diffColor = sf::Color(100, 255, 100); break;
            case Difficulty::Normal: diffText = "[2] Normal"; diffColor = sf::Color(255, 255, 100); break;
            case Difficulty::Hard:   diffText = "[3] Hard"; diffColor = sf::Color(255, 165, 0); break;
            case Difficulty::Hell:   diffText = "[4] Hell"; diffColor = sf::Color(255, 50, 50); break;
        }
        renderer.drawText(diffText, centerX, centerY + 75.0f, 22, diffColor, 3);
        renderer.drawText("Press 1-4 to change", centerX, centerY + 110.0f, 22, sf::Color(120, 120, 120), 3);
    }
    
    // Instructions
    renderer.drawText("Press Enter to Start", centerX, centerY + 160.0f, 24, sf::Color(200, 255, 200), 3);
    renderer.drawText("Press Esc to Quit", centerX, centerY + 200.0f, 24, sf::Color(180, 180, 180), 3);

    renderer.drawLogo(centerX, centerY + 300.0f);
    
    renderer.drawMessage();
    renderer.display();
}

} // namespace Quoridor
