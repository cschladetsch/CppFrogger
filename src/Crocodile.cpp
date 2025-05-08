#include "Crocodile.hpp"
#include <iostream>

Crocodile::Crocodile(float x, float y, float speed, const sf::Vector2f& direction)
    : GameEntity(x, y, 100.0f, 40.0f, sf::Color(20, 100, 20), EntityType::CROCODILE), 
      speed(speed), direction(direction), animationTimer(0), mouthOpen(false), dangerous(false) {
    
    // Create a more crocodile-like shape
    shape.setPointCount(7);
    
    // Main body points
    shape.setPoint(0, sf::Vector2f(-50.0f, -15.0f)); // Top left
    shape.setPoint(1, sf::Vector2f(30.0f, -15.0f));  // Top right before head
    shape.setPoint(2, sf::Vector2f(50.0f, -5.0f));   // Head top
    shape.setPoint(3, sf::Vector2f(50.0f, 5.0f));    // Head bottom
    shape.setPoint(4, sf::Vector2f(30.0f, 15.0f));   // Bottom right before head
    shape.setPoint(5, sf::Vector2f(-50.0f, 15.0f));  // Bottom left
    shape.setPoint(6, sf::Vector2f(-40.0f, 0.0f));   // Tail
    
    // Rotate based on direction
    if (direction.x < 0) {
        setRotation(180);
    }
}

void Crocodile::update(float deltaTime) {
    if (!active) return;
    
    // Move crocodile
    position += direction * speed * deltaTime;
    
    // Update animation
    updateAnimation(deltaTime);
    
    // Check if crocodile is out of bounds
    checkBounds(800.0f); // Assuming 800 is the screen width
}

void Crocodile::updateAnimation(float deltaTime) {
    // Every 2 seconds, toggle between dangerous and safe
    animationTimer += deltaTime;
    
    if (animationTimer >= 2.0f) {
        animationTimer = 0;
        mouthOpen = !mouthOpen;
        dangerous = mouthOpen;
        
        // Darker green when mouth is open (dangerous)
        if (mouthOpen) {
            shape.setFillColor(sf::Color(0, 80, 0)); // Darker green
        } else {
            shape.setFillColor(sf::Color(20, 100, 20)); // Normal green
        }
    }
}

void Crocodile::checkBounds(float maxX) {
    if ((direction.x > 0 && position.x > maxX + 50.0f) ||
        (direction.x < 0 && position.x < -50.0f)) {
        setActive(false);
    }
}

sf::Vector2f Crocodile::getMovement(float deltaTime) const {
    return direction * speed * deltaTime;
}

bool Crocodile::isDangerous() const {
    return dangerous;
}