#include "controller/MenuScene.hpp"
#include "controller/GameScene.hpp"

namespace Quoridor {

MenuScene::MenuScene(SceneManager& manager)
    : manager(manager), renderer(manager.getRenderer()),
      selectedMode(GameMode::HumanVsHuman),
      selectedDifficulty(Difficulty::Normal) {}

void MenuScene::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        renderer.close();
        return;
    }

    if (event.is<sf::Event::KeyPressed>()) {
        const auto key = event.getIf<sf::Event::KeyPressed>()->code;
        
        // Enter - Start game with selected mode
        if (key == sf::Keyboard::Key::Enter) {
            manager.setScene(std::make_unique<GameScene>(manager, selectedMode, selectedDifficulty), GameState::Playing);
            return;
        }
        
        // Escape - Quit
        if (key == sf::Keyboard::Key::Escape) {
            renderer.close();
            return;
        }
        
        // H - Human vs Human
        if (key == sf::Keyboard::Key::H) {
            selectedMode = GameMode::HumanVsHuman;
            renderer.showMessage("Mode: Human vs Human", {100, 255, 100}, 1.5f);
            return;
        }
        
        // A - Human vs AI
        if (key == sf::Keyboard::Key::A) {
            selectedMode = GameMode::HumanVsAI;
            renderer.showMessage("Mode: Human vs AI", {255, 200, 100}, 1.5f);
            return;
        }
        
        // 1-4 - AI Difficulty (only if AI mode selected)
        if (selectedMode == GameMode::HumanVsAI) {
            if (key == sf::Keyboard::Key::Num1) {
                selectedDifficulty = Difficulty::Easy;
                renderer.showMessage("AI Difficulty: Easy", {100, 255, 100}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num2) {
                selectedDifficulty = Difficulty::Normal;
                renderer.showMessage("AI Difficulty: Normal", {255, 255, 100}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num3) {
                selectedDifficulty = Difficulty::Hard;
                renderer.showMessage("AI Difficulty: Hard", {255, 165, 0}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num4) {
                selectedDifficulty = Difficulty::Hell;
                renderer.showMessage("AI Difficulty: Hell", {255, 50, 50}, 1.5f);
                return;
            }
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
    renderer.drawText("ENSTQuoridor", centerX, centerY - 200.0f, 48, sf::Color::White, 1);
    
    // Game Mode Selection
    renderer.drawText("Select Game Mode:", centerX, centerY - 100.0f, 24, sf::Color(200, 200, 200), 2);
    
    sf::Color hvhColor = (selectedMode == GameMode::HumanVsHuman) ? sf::Color(100, 255, 100) : sf::Color(150, 150, 150);
    sf::Color hvaiColor = (selectedMode == GameMode::HumanVsAI) ? sf::Color(255, 200, 100) : sf::Color(150, 150, 150);
    
    renderer.drawText("[H] Human vs Human", centerX, centerY - 50.0f, 22, hvhColor, 3);
    renderer.drawText("[A] Human vs AI", centerX, centerY - 15.0f, 22, hvaiColor, 3);
    
    // Difficulty selection (only show if AI mode)
    if (selectedMode == GameMode::HumanVsAI) {
        renderer.drawText("AI Difficulty:", centerX, centerY + 30.0f, 20, sf::Color(200, 200, 200), 2);
        
        std::string diffText;
        sf::Color diffColor;
        switch(selectedDifficulty) {
            case Difficulty::Easy:   diffText = "[1] Easy"; diffColor = sf::Color(100, 255, 100); break;
            case Difficulty::Normal: diffText = "[2] Normal"; diffColor = sf::Color(255, 255, 100); break;
            case Difficulty::Hard:   diffText = "[3] Hard"; diffColor = sf::Color(255, 165, 0); break;
            case Difficulty::Hell:   diffText = "[4] Hell"; diffColor = sf::Color(255, 50, 50); break;
        }
        renderer.drawText(diffText, centerX, centerY + 60.0f, 18, diffColor, 3);
        renderer.drawText("Press 1-4 to change", centerX, centerY + 85.0f, 14, sf::Color(120, 120, 120), 3);
    }
    
    // Instructions
    renderer.drawText("Press Enter to Start", centerX, centerY + 130.0f, 24, sf::Color(200, 255, 200), 3);
    renderer.drawText("Press Esc to Quit", centerX, centerY + 165.0f, 20, sf::Color(180, 180, 180), 3);

    renderer.drawLogo(centerX, centerY + 250.0f);
    
    renderer.drawMessage();
    renderer.display();
}

} // namespace Quoridor
