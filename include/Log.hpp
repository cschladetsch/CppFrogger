#pragma once

#include "GameEntity.hpp"

class Log : public GameEntity {
private:
    float speed;
    sf::Vector2f direction;

public:
    Log(float x, float y, float speed, const sf::Vector2f& direction);
    
    void update(float deltaTime) override;
    void checkBounds(float maxX);
    sf::Vector2f getMovement(float deltaTime) const;
};
