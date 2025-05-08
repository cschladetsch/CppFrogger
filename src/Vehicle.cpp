#include "Vehicle.hpp"
#include <iostream>

Vehicle::Vehicle(float x, float y, float speed, const sf::Vector2f& direction)
    : GameEntity(x, y, 60.0f, 30.0f, sf::Color::Red, EntityType::VEHICLE), 
      speed(speed), direction(direction) {
    
    // Rotate shape based on direction
    if (direction.x < 0) {
        setRotation(180); // Face left
    }
}

void Vehicle::update(float deltaTime) {
    if (!active) return;
    
    // Move vehicle
    position += direction * speed * deltaTime;
    
    // Check if vehicle is out of bounds
    checkBounds(800.0f); // Assuming 800 is the screen width
}

void Vehicle::checkBounds(float maxX) {
    if ((direction.x > 0 && position.x > maxX + 30.0f) ||
        (direction.x < 0 && position.x < -30.0f)) {
        setActive(false);
    }
}
