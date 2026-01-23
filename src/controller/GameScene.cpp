#include "controller/GameScene.hpp"
#include "controller/GameOverScene.hpp"
#include "controller/MenuScene.hpp"
#include <string>

namespace Quoridor {

GameScene::GameScene(SceneManager& manager, GameMode mode, Difficulty difficulty)
    : manager(manager),
      renderer(manager.getRenderer()),
      board(),
      state(),
      rules(),
      gameView(),
      inputHandler(board, state, rules, renderer),
      gameMode(mode),
      aiDifficulty(difficulty),
      aiThinking(false),
      aiDelayTimer(0.0f) {
    int currentPlayer = state.getCurrentPlayer();
    std::string playerName = "Player " + std::to_string(currentPlayer + 1);
    
    if (gameMode == GameMode::HumanVsAI) {
        renderer.showMessage(playerName + " Turn (Human vs AI mode)", {255,255,255}, -1.0f);
    } else {
        renderer.showMessage(playerName + " Turn, select pawn to start moving or press w to place wall", {255,255,255}, -1.0f);
    }
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
        
        // Allow difficulty change during game in AI mode
        if (gameMode == GameMode::HumanVsAI) {
            if (key == sf::Keyboard::Key::Num1) {
                aiDifficulty = Difficulty::Easy;
                renderer.showMessage("AI Difficulty: Easy", {100, 255, 100}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num2) {
                aiDifficulty = Difficulty::Normal;
                renderer.showMessage("AI Difficulty: Normal", {255, 255, 100}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num3) {
                aiDifficulty = Difficulty::Hard;
                renderer.showMessage("AI Difficulty: Hard", {255, 165, 0}, 1.5f);
                return;
            }
            if (key == sf::Keyboard::Key::Num4) {
                aiDifficulty = Difficulty::Hell;
                renderer.showMessage("AI Difficulty: Hell", {255, 50, 50}, 1.5f);
                return;
            }
        }
    }

    // Block input during AI's turn
    if (gameMode == GameMode::HumanVsAI && 
        state.getCurrentPlayer() == 1 && 
        state.getGameStatus() == GameStatus::Playing) {
        return; // Don't process human input during AI turn
    }

    inputHandler.handleInput(event, renderer.getWindow());
}

void GameScene::update(float deltaTime) {
    renderer.updateMessage(deltaTime);
    checkForGameOver();
    
    // AI turn logic (only in HumanVsAI mode)
    if (gameMode == GameMode::HumanVsAI && 
        state.getCurrentPlayer() == 1 && 
        state.getGameStatus() == GameStatus::Playing &&
        !aiThinking) {
        
        aiDelayTimer += deltaTime;
        
        if (aiDelayTimer >= AI_DELAY) {
            executeAITurn();
            aiDelayTimer = 0.0f;
        }
    }
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

void GameScene::executeAITurn() {
    aiThinking = true;
    renderer.showMessage("AI is thinking...", {255, 200, 100}, -1.0f);
    
    AI ai;
    Move aiMove = ai.getBestMove(board, state, aiDifficulty);
    
    applyAIMove(aiMove);
    
    // Check for AI victory
    if (rules.checkVictory(board, 1)) {
        state.setGameStatus(GameStatus::Player2Won);
        renderer.showMessage("AI Wins! Press R to restart or Esc for menu", {255, 100, 100}, -1.0f);
    } else {
        // Switch back to human player
        state.switchPlayer();
        renderer.showMessage("Player 1 Turn - Your move", {255, 255, 255}, -1.0f);
    }
    
    aiThinking = false;
}

void GameScene::applyAIMove(const Move& aiMove) {
    if (aiMove.isPawnMove()) {
        board.movePawn(1, aiMove.pawnDest);
        std::string msg = "AI moved pawn to (" + std::to_string(aiMove.pawnDest.x) + "," + std::to_string(aiMove.pawnDest.y) + ")";
        renderer.showMessage(msg, {150, 150, 255}, 1.5f);
    } else if (aiMove.isWallPlacement()) {
        board.placeWall(aiMove.wall, 1);
        std::string orientStr = (aiMove.wall.orientation == Orientation::Horizontal) ? "horizontal" : "vertical";
        std::string msg = "AI placed " + orientStr + " wall";
        renderer.showMessage(msg, {150, 150, 255}, 1.5f);
    }
}

} // namespace Quoridor
