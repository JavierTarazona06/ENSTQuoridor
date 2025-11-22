#include "model/State.hpp"

namespace Quoridor {

    State::State(Board& board) 
        : board_(board), 
          currentPlayer_(0), 
          status_(GameStatus::Playing) {
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

    Board& State::getBoard() {
        return board_;
    }

    const Board& State::getBoard() const {
        return board_;
    }

} // namespace Quoridor
