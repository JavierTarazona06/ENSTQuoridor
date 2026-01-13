#pragma once

#include "model/Board.hpp"
#include <optional>

namespace Quoridor {

    enum class GameStatus {
        Playing,
        Player1Won,
        Player2Won
    };

    class State {
    public:
        /**
         * @brief Constructor: Initializes the game state with player 0 starting.
         */
        State();

        /**
         * @brief Resets the game state to initial conditions.
         */
        void resetGame();

        /**
         * @brief Gets the current active player index (0 or 1).
         * @return int Player index
         */
        int getCurrentPlayer() const;

        /**
         * @brief Switches the active player.
         */
        void switchPlayer();

        /**
         * @brief Gets the current game status.
         * @return GameStatus
         */
        GameStatus getGameStatus() const;

        /**
         * @brief Sets the game status.
         * @param status New status
         */
        void setGameStatus(GameStatus status);

        // Selection state for UI
        void setSelectedPawn(std::optional<Position> pos);
        std::optional<Position> getSelectedPawn() const;
        
        // Wall preview for placement mode
        void setPreviewWall(const std::optional<Wall>& wall);
        std::optional<Wall> getPreviewWall() const;

    private:
        int currentPlayer_; // 0 or 1
        GameStatus status_;
        std::optional<Position> selectedPawn_;
        std::optional<Wall> previewWall_;
    };

} // namespace Quoridor
