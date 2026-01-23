#pragma once

#include "controller/Scene.hpp"
#include "controller/SceneManager.hpp"
#include "controller/InputHandler.hpp"
#include "view/GameView.hpp"
#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"
#include "model/AI.hpp"

namespace Quoridor {

class GameScene : public Scene {
public:
    explicit GameScene(SceneManager& manager, GameMode mode = GameMode::HumanVsHuman, Difficulty difficulty = Difficulty::Normal);
    ~GameScene() override = default;

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;

private:
    void checkForGameOver();
    void executeAITurn();
    void applyAIMove(const Move& aiMove);

    SceneManager& manager;
    Render2D& renderer;
    Board board;
    State state;
    Rules rules;
    GameView gameView;
    InputHandler inputHandler;
    
    // AI-related members
    GameMode gameMode;
    Difficulty aiDifficulty;
    bool aiThinking;
    float aiDelayTimer;
    static constexpr float AI_DELAY = 0.5f;
};

} // namespace Quoridor
