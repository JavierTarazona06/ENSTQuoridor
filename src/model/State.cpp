#include "model/State.hpp"
#include "model/PlayerId.hpp"
#include <string>

namespace Quoridor {

    State::State() 
        : currentPlayer_(0), 
          status_(GameStatus::Playing) {
    }

    void State::resetGame() {
        currentPlayer_ = 0;
        status_ = GameStatus::Playing;
        selectedPawn_.reset();
        previewWall_.reset();
    }

    int State::getCurrentPlayer() const {
        return currentPlayer_;
    }

    void State::switchPlayer() {
        currentPlayer_ = (currentPlayer_ + 1) % 2;
    }

    GameStatus State::getGameStatus() const {
        return status_;
    }

    void State::setGameStatus(GameStatus status) {
        status_ = status;
    }

    void State::setSelectedPawn(std::optional<Position> pos) {
        selectedPawn_ = pos;
    }

    std::optional<Position> State::getSelectedPawn() const {
        return selectedPawn_;
    }
    
    void State::setPreviewWall(const std::optional<Wall>& wall) {
        previewWall_ = wall;
    }

    std::optional<Wall> State::getPreviewWall() const {
        return previewWall_;
    }

    std::string State::getPlayerName() const {
        int index = getCurrentPlayer();
        if (index < 0 || index >= static_cast<int>(PLAYER_NAMES.size())) {
            return "Unknown";
        }
        return std::string(PLAYER_NAMES[static_cast<std::size_t>(index)]);
    }

} // namespace Quoridor
