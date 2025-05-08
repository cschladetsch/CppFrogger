#pragma once

#include <SFML/Graphics.hpp>

enum class EntityType {
    PLAYER,
    VEHICLE,
    LOG,
    CROCODILE
};

class GameEntity {
protected:
    sf::Vector2f position;
    sf::ConvexShape shape;
    bool active = true;
    EntityType type;

public:
    GameEntity(float x, float y, float width, float height, sf::Color color, EntityType type);
    virtual ~GameEntity() = default;
    
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window);
    
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    bool isActive() const;
    void setActive(bool active);
    void setRotation(float angle);
    EntityType getType() const;
};
