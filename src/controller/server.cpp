#include "controller/server.hpp"
#include "view/GameView.hpp"
#include <memory>
#include <optional>

namespace Quoridor {

Server::Server() 
    : board_(), state_(), rules_(), render_(), inputHandler_(board_, state_, rules_, render_), current_view_(std::make_unique<GameView>()) {
}

Server::~Server() {
}

void Server::startGame() {
    // Display initial player turn message
    std::string playerName = state_.getPlayerName();
    render_.showMessage(playerName + " Turn, select pawn to start moving or press w to place wall", {255,255,255}, -1.0f);
    
    // Main game loop
    while (render_.isOpen()) {
        const float deltaTime = clock_.restart().asSeconds();
        handleEvents();
        update(deltaTime);
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

void Server::update(float deltaTime) {
    // Update message box timer
    render_.updateMessage(deltaTime);
    
    // Update game logic here
    // This is where game state changes will happen
}

void Server::render() {
    // Clear screen
    render_.clear();
    
    // Draw using current view
    current_view_->render(render_, board_, state_);
    
    // Draw message box
    render_.drawMessage();
    
    // Display everything
    render_.display();
}

} // namespace Quoridor
