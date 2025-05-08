#include "Player.hpp"
#include <iostream>

Player::Player(float x, float y)
    : GameEntity(x, y, 30.0f, 30.0f, sf::Color::Green, EntityType::PLAYER), 
      speed(200.0f), lives(3), onLog(false), logMovement(0.0f, 0.0f) {
    // Initialize as triangle shape for the frog (pointing upward)
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0, -15));    // Top
    shape.setPoint(1, sf::Vector2f(-15, 15));   // Bottom left
    shape.setPoint(2, sf::Vector2f(15, 15));    // Bottom right
}

void Player::update(float deltaTime) {
    if (!active) return;
    
    handleInput();
    
    // If on a log, move with it
    if (onLog) {
        position += logMovement;
    }
    
    // Keep player within screen bounds
    if (position.x < 0) position.x = 0;
    if (position.x > 800) position.x = 800;
    if (position.y < 0) position.y = 0;
    if (position.y > 600) position.y = 600;
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        position.y -= speed * 0.016f; // Approximately one frame at 60fps
        setRotation(0); // Rotate to face up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        position.y += speed * 0.016f;
        setRotation(180); // Rotate to face down
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        position.x -= speed * 0.016f;
        setRotation(270); // Rotate to face left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        position.x += speed * 0.016f;
        setRotation(90); // Rotate to face right
    }
}

void Player::moveWithLog(const sf::Vector2f& movement) {
    logMovement = movement;
}

void Player::resetPosition() {
    position = sf::Vector2f(400, 550); // Bottom center of the screen
    setRotation(0); // Reset rotation
}

void Player::loseLife() {
    lives--;
    if (lives <= 0) {
        setActive(false);
    }
    resetPosition();
}

int Player::getLives() const {
    return lives;
}

bool Player::isOnLog() const {
    return onLog;
}

void Player::setOnLog(bool onLog) {
    this->onLog = onLog;
    if (!onLog) {
        logMovement = sf::Vector2f(0.0f, 0.0f);
    }
}
