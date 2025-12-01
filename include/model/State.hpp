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
         * @brief Constructor: Initializes the game state with a given board and player 0 starting.
         * @param board Reference to the Board instance to use
         */
        State(Board& board);

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

        /**
         * @brief Accessor for the Board instance.
         * @return Board&
         */
        Board& getBoard();
        const Board& getBoard() const;

        // Selection state for UI
        void setSelectedPawn(std::optional<Position> pos);
        std::optional<Position> getSelectedPawn() const;

    private:
        Board& board_;
        int currentPlayer_; // 0 or 1
        GameStatus status_;
        std::optional<Position> selectedPawn_;
    };

} // namespace Quoridor
