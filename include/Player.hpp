#pragma once

#include "GameEntity.hpp"

class Player : public GameEntity {
private:
    float speed;
    int lives;
    bool onLog;
    sf::Vector2f logMovement;

public:
    Player(float x, float y);
    
    void update(float deltaTime) override;
    void handleInput();
    void moveWithLog(const sf::Vector2f& movement);
    void resetPosition();
    void loseLife();
    int getLives() const;
    bool isOnLog() const;
    void setOnLog(bool onLog);
};
