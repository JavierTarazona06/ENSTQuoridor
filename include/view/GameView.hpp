#pragma once

#include "view/View.hpp"
#include "view/Button.hpp"
#include "model/AI.hpp"

namespace Quoridor {

class GameView : public View {
public:
    GameView();

    // Override the render method from View base class
    void render(Render2D& render, const Board& board, const State& state, GameMode mode = GameMode::HumanVsHuman, Difficulty difficulty = Difficulty::Normal) override;

    /**
     * @brief Handle button clicks
     * @param x Click X coordinate
     * @param y Click Y coordinate
     * @return true if a button was clicked
     */
    bool handleButtonClick(float x, float y);

    /**
     * @brief Update button hover state based on mouse position
     * @param x Mouse X coordinate
     * @param y Mouse Y coordinate
     */
    void updateButtonHover(float x, float y);

    /**
     * @brief Get home button reference
     * @return Reference to home button
     */
    Button& getHomeButton() { return homeButton; }

    /**
     * @brief Get restart button reference
     * @return Reference to restart button
     */
    Button& getRestartButton() { return restartButton; }

private:
    // Draw game mode and difficulty info box
    void drawGameModeInfoBox(Render2D& render, GameMode mode, Difficulty difficulty);
    
    // Draw UI buttons
    void drawButtons(Render2D& render);

    Button homeButton;
    Button restartButton;
};

} // namespace Quoridor
