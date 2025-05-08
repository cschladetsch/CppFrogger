#pragma once

#include "GameEntity.hpp"

class Vehicle : public GameEntity {
private:
    float speed;
    sf::Vector2f direction;

public:
    Vehicle(float x, float y, float speed, const sf::Vector2f& direction);
    
    void update(float deltaTime) override;
    void checkBounds(float maxX);
};
