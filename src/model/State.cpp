#include "model/State.hpp"

namespace Quoridor {

    State::State() 
        : currentPlayer_(0), 
          status_(GameStatus::Playing) {
    }

    void State::resetGame() {
        currentPlayer_ = 0;
        status_ = GameStatus::Playing;
        selectedPawn_.reset();
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

} // namespace Quoridor
