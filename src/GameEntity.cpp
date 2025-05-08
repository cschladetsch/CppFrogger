#include "GameEntity.hpp"

GameEntity::GameEntity(float x, float y, float width, float height, sf::Color color, EntityType type)
    : position(x, y), type(type) {
    // By default make a rectangular shape with 4 points
    shape.setPointCount(4);
    shape.setPoint(0, sf::Vector2f(-width/2, -height/2));  // Top left
    shape.setPoint(1, sf::Vector2f(width/2, -height/2));   // Top right
    shape.setPoint(2, sf::Vector2f(width/2, height/2));    // Bottom right
    shape.setPoint(3, sf::Vector2f(-width/2, height/2));   // Bottom left
    
    shape.setFillColor(color);
    shape.setPosition(position);
}

void GameEntity::render(sf::RenderWindow& window) {
    if (active) {
        shape.setPosition(position);
        window.draw(shape);
    }
}

sf::Vector2f GameEntity::getPosition() const {
    return position;
}

sf::FloatRect GameEntity::getBounds() const {
    return shape.getGlobalBounds();
}

bool GameEntity::isActive() const {
    return active;
}

void GameEntity::setActive(bool active) {
    this->active = active;
}

void GameEntity::setRotation(float angle) {
    shape.setRotation(angle);
}

EntityType GameEntity::getType() const {
    return type;
}
