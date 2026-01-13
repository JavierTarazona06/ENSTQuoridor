#pragma once

#include "model/Board.hpp"
#include "model/State.hpp"
#include "model/Rules.hpp"
#include "controller/InputHandler.hpp"
#include "view/Render2D.hpp"
#include "view/View.hpp"
#include <SFML/System/Clock.hpp>
#include <memory>

namespace Quoridor {

class Server {
public:
    Server();
    ~Server();

    // Start the game loop
    void startGame();

private:
    Board board_;
    State state_;
    Rules rules_;
    InputHandler inputHandler_;
    Render2D render_;
    std::unique_ptr<View> current_view_;
    sf::Clock clock_;

    // Handle game events
    void handleEvents();
    
    // Update game state
    void update(float deltaTime);
    
    // Render the game
    void render();
};

} // namespace Quoridor
