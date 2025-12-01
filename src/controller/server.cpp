#include "controller/server.hpp"
#include "view/GameView.hpp"
#include <memory>
#include <optional>

namespace Quoridor {

Server::Server() 
    : board_(), state_(), rules_(), inputHandler_(board_, state_, rules_), render_(), current_view_(std::make_unique<GameView>()) {
}

Server::~Server() {
}

void Server::startGame() {
    // Main game loop
    while (render_.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Server::handleEvents() {
    // Process all pending events
    while (const std::optional event = render_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            render_.close();
        }
        
        // Pass event to input handler
        inputHandler_.handleInput(*event, render_.getWindow());
    }
}

void Server::update() {
    // Update game logic here
    // This is where game state changes will happen
}

void Server::render() {
    // Clear screen
    render_.clear();
    
    // Draw using current view
    current_view_->render(render_, board_, state_);
    
    // Display everything
    render_.display();
}

} // namespace Quoridor
