#pragma once

#include "GameEntity.hpp"

class Crocodile : public GameEntity {
private:
    float speed;
    sf::Vector2f direction;
    float animationTimer;
    bool mouthOpen;
    bool dangerous;

public:
    Crocodile(float x, float y, float speed, const sf::Vector2f& direction);
    
    void update(float deltaTime) override;
    void checkBounds(float maxX);
    sf::Vector2f getMovement(float deltaTime) const;
    bool isDangerous() const;
    void updateAnimation(float deltaTime);
};