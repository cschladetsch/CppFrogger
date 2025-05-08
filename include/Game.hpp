#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

#include "Player.hpp"
#include "Vehicle.hpp"
#include "Log.hpp"
#include "Crocodile.hpp"

enum class GameState {
    START_SCREEN,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text titleText;
    sf::Text instructionText;
    
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    std::vector<std::unique_ptr<Log>> logs;
    std::vector<std::unique_ptr<Crocodile>> crocodiles;
    
    sf::RectangleShape roadArea;
    sf::RectangleShape waterArea;
    sf::RectangleShape grassArea;
    sf::RectangleShape goalArea;
    sf::RectangleShape menuBackground;
    
    std::mt19937 rng;
    
    int score;
    GameState gameState;
    float logSpawnTimer;
    float vehicleSpawnTimer;
    float crocodileSpawnTimer;
    
    void processEvents();
    void update(float deltaTime);
    void render();
    void checkCollisions();
    void spawnVehicle();
    void spawnLog();
    void spawnCrocodile();
    void resetGame();
    void renderStartScreen();
    void renderPauseMenu();
    void renderGameOverScreen();
    
public:
    Game();
    
    void run();
};
