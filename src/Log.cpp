#include "Log.hpp"
#include <iostream>

Log::Log(float x, float y, float speed, const sf::Vector2f& direction)
    : GameEntity(x, y, 100.0f, 30.0f, sf::Color(139, 69, 19), EntityType::LOG), 
      speed(speed), direction(direction) {
}

void Log::update(float deltaTime) {
    if (!active) return;
    
    // Move log
    position += direction * speed * deltaTime;
    
    // Check if log is out of bounds
    checkBounds(800.0f); // Assuming 800 is the screen width
}

void Log::checkBounds(float maxX) {
    if ((direction.x > 0 && position.x > maxX + 50.0f) ||
        (direction.x < 0 && position.x < -50.0f)) {
        setActive(false);
    }
}

sf::Vector2f Log::getMovement(float deltaTime) const {
    return direction * speed * deltaTime;
}
