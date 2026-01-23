#pragma once

#include "model/State.hpp"
#include "model/AI.hpp"

namespace Quoridor {

/**
 * @brief Structure to store game configuration parameters before launching a game.
 * 
 * This struct encapsulates all settings needed to configure a game session,
 * including the game mode (PvP or PvE) and AI difficulty level.
 */
struct GameConfig {
    GameMode mode;          ///< Game mode: HumanVsHuman (PvP) or HumanVsAI (PvE)
    Difficulty difficulty;  ///< AI difficulty level (only used in HumanVsAI mode)
    
    /**
     * @brief Construct a new GameConfig with default or specified values.
     * @param m Game mode (default: HumanVsHuman)
     * @param d AI difficulty (default: Normal)
     */
    GameConfig(GameMode m = GameMode::HumanVsHuman, Difficulty d = Difficulty::Normal)
        : mode(m), difficulty(d) {}
    
    /// @brief Check if the game is in Player vs AI mode
    bool isAIMode() const { return mode == GameMode::HumanVsAI; }
    
    /// @brief Check if the game is in Player vs Player mode
    bool isPvPMode() const { return mode == GameMode::HumanVsHuman; }
};

} // namespace Quoridor
