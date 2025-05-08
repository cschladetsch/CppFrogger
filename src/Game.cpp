#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

Game::Game()
    : window(sf::VideoMode(800, 600), "C++23 Frogger Clone"),
      score(0),
      gameState(GameState::START_SCREEN),
      logSpawnTimer(0),
      vehicleSpawnTimer(0),
      crocodileSpawnTimer(0) {
    
    // Set up RNG
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
    
    // Set up background areas
    roadArea.setSize(sf::Vector2f(800.0f, 200.0f));
    roadArea.setPosition(0, 350);
    roadArea.setFillColor(sf::Color(50, 50, 50)); // Dark gray for road
    
    waterArea.setSize(sf::Vector2f(800.0f, 200.0f));
    waterArea.setPosition(0, 150);
    waterArea.setFillColor(sf::Color(0, 0, 200)); // Blue for water
    
    grassArea.setSize(sf::Vector2f(800.0f, 300.0f));
    grassArea.setPosition(0, 550);
    grassArea.setFillColor(sf::Color(0, 150, 0)); // Green for grass (bottom area)
    
    goalArea.setSize(sf::Vector2f(800.0f, 100.0f));
    goalArea.setPosition(0, 0);
    goalArea.setFillColor(sf::Color(0, 150, 0)); // Green for grass (goal area)
    
    // Set up semi-transparent background for menus
    menuBackground.setSize(sf::Vector2f(400.0f, 300.0f));
    menuBackground.setPosition(200.0f, 150.0f);
    menuBackground.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black
    
    // Try to load font from different possible locations
    bool fontLoaded = false;
    
    // Try to load from fonts directory
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("fonts/any_font.ttf");
    }
    
    // Try system fonts on Linux
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    }
    
    // Try system fonts on Windows
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
    }
    
    // Try system fonts on macOS
    if (!fontLoaded) {
        fontLoaded = font.loadFromFile("/Library/Fonts/Arial.ttf");
    }
    
    // If no font could be loaded, warn but continue
    if (!fontLoaded) {
        std::cerr << "Warning: Could not load any font. Text rendering may not work correctly." << std::endl;
        std::cerr << "Please add a TTF font to the 'fonts' directory." << std::endl;
    }
    
    // Set up game text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(650, 10);
    
    // Set up menu text
    titleText.setFont(font);
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Green);
    titleText.setString("C++23 FROGGER");
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
    titleText.setPosition(400, 200);
    
    instructionText.setFont(font);
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    
    // Create player
    player = std::make_unique<Player>(400, 550);
    
    // Limit framerate
    window.setFramerateLimit(60);
}

void Game::run() {
    resetGame();
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        processEvents();
        
        if (gameState == GameState::PLAYING) {
            update(deltaTime);
            
            // Spawn vehicles with timer
            vehicleSpawnTimer += deltaTime;
            if (vehicleSpawnTimer >= 0.5f) {
                if (std::uniform_real_distribution<float>(0, 1)(rng) < 0.3f) {
                    spawnVehicle();
                }
                vehicleSpawnTimer = 0;
            }
            
            // Spawn logs with timer - more frequently than before
            logSpawnTimer += deltaTime;
            if (logSpawnTimer >= 1.0f) {
                if (std::uniform_real_distribution<float>(0, 1)(rng) < 0.5f) {
                    spawnLog();
                }
                logSpawnTimer = 0;
            }
            
            // Spawn crocodiles occasionally
            crocodileSpawnTimer += deltaTime;
            if (crocodileSpawnTimer >= 3.0f) {
                if (std::uniform_real_distribution<float>(0, 1)(rng) < 0.3f) {
                    spawnCrocodile();
                }
                crocodileSpawnTimer = 0;
            }
        }
        
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            switch (gameState) {
                case GameState::START_SCREEN:
                    if (event.key.code == sf::Keyboard::Space || 
                        event.key.code == sf::Keyboard::Return) {
                        gameState = GameState::PLAYING;
                    }
                    break;
                    
                case GameState::PLAYING:
                    if (event.key.code == sf::Keyboard::P || 
                        event.key.code == sf::Keyboard::Escape) {
                        gameState = GameState::PAUSED;
                    } else if (event.key.code == sf::Keyboard::R) {
                        resetGame();
                    }
                    break;
                    
                case GameState::PAUSED:
                    if (event.key.code == sf::Keyboard::P || 
                        event.key.code == sf::Keyboard::Escape) {
                        gameState = GameState::PLAYING;
                    } else if (event.key.code == sf::Keyboard::R) {
                        resetGame();
                        gameState = GameState::PLAYING;
                    } else if (event.key.code == sf::Keyboard::Q) {
                        window.close();
                    }
                    break;
                    
                case GameState::GAME_OVER:
                    if (event.key.code == sf::Keyboard::R) {
                        resetGame();
                        gameState = GameState::PLAYING;
                    } else if (event.key.code == sf::Keyboard::Q) {
                        window.close();
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        gameState = GameState::START_SCREEN;
                    }
                    break;
            }
        }
    }
}

void Game::update(float deltaTime) {
    // Update player
    player->update(deltaTime);
    
    // Update vehicles
    for (auto& vehicle : vehicles) {
        vehicle->update(deltaTime);
    }
    
    // Update logs
    for (auto& log : logs) {
        log->update(deltaTime);
    }
    
    // Update crocodiles
    for (auto& croc : crocodiles) {
        croc->update(deltaTime);
    }
    
    // Remove inactive entities
    vehicles.erase(
        std::remove_if(vehicles.begin(), vehicles.end(),
            [](const auto& vehicle) { return !vehicle->isActive(); }),
        vehicles.end());
    
    logs.erase(
        std::remove_if(logs.begin(), logs.end(),
            [](const auto& log) { return !log->isActive(); }),
        logs.end());
        
    crocodiles.erase(
        std::remove_if(crocodiles.begin(), crocodiles.end(),
            [](const auto& croc) { return !croc->isActive(); }),
        crocodiles.end());
    
    // Check collisions
    checkCollisions();
    
    // Update text
    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + std::to_string(player->getLives()));
    
    // Check if player reached goal
    if (player->getPosition().y < 50) {
        score += 100;
        player->resetPosition();
    }
    
    // Check if game over
    if (!player->isActive()) {
        gameState = GameState::GAME_OVER;
    }
}

void Game::render() {
    window.clear(sf::Color(0, 0, 0)); // Black background
    
    switch (gameState) {
        case GameState::START_SCREEN:
            renderStartScreen();
            break;
            
        case GameState::PLAYING:
            // Draw background elements
            window.draw(goalArea);
            window.draw(waterArea);
            window.draw(roadArea);
            window.draw(grassArea);
            
            // Draw logs
            for (auto& log : logs) {
                log->render(window);
            }
            
            // Draw crocodiles
            for (auto& croc : crocodiles) {
                croc->render(window);
            }
            
            // Draw player
            player->render(window);
            
            // Draw vehicles
            for (auto& vehicle : vehicles) {
                vehicle->render(window);
            }
            
            // Draw UI
            window.draw(scoreText);
            window.draw(livesText);
            break;
            
        case GameState::PAUSED:
            // Draw game state (so player can see what's happening)
            window.draw(goalArea);
            window.draw(waterArea);
            window.draw(roadArea);
            window.draw(grassArea);
            
            for (auto& log : logs) {
                log->render(window);
            }
            
            for (auto& croc : crocodiles) {
                croc->render(window);
            }
            
            player->render(window);
            
            for (auto& vehicle : vehicles) {
                vehicle->render(window);
            }
            
            renderPauseMenu();
            break;
            
        case GameState::GAME_OVER:
            // Draw game state
            window.draw(goalArea);
            window.draw(waterArea);
            window.draw(roadArea);
            window.draw(grassArea);
            
            for (auto& log : logs) {
                log->render(window);
            }
            
            for (auto& croc : crocodiles) {
                croc->render(window);
            }
            
            for (auto& vehicle : vehicles) {
                vehicle->render(window);
            }
            
            renderGameOverScreen();
            break;
    }
    
    window.display();
}

void Game::renderStartScreen() {
    // Set instruction text
    instructionText.setString("Press SPACE or ENTER to start\n\nControls:\nArrow Keys - Move\nP/ESC - Pause\nR - Restart");
    
    // Center the instruction text
    sf::FloatRect textRect = instructionText.getLocalBounds();
    instructionText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    instructionText.setPosition(400, 350);
    
    // Draw title and instructions
    window.draw(titleText);
    window.draw(instructionText);
}

void Game::renderPauseMenu() {
    // Draw semi-transparent background
    window.draw(menuBackground);
    
    // Update text
    titleText.setString("PAUSED");
    titleText.setPosition(400, 200);
    
    instructionText.setString("Press P/ESC to continue\nPress R to restart\nPress Q to quit");
    
    // Center the instruction text
    sf::FloatRect textRect = instructionText.getLocalBounds();
    instructionText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    instructionText.setPosition(400, 300);
    
    // Draw title and instructions
    window.draw(titleText);
    window.draw(instructionText);
}

void Game::renderGameOverScreen() {
    // Draw semi-transparent background
    window.draw(menuBackground);
    
    // Update text
    titleText.setString("GAME OVER");
    titleText.setPosition(400, 200);
    titleText.setFillColor(sf::Color::Red);
    
    instructionText.setString("Final Score: " + std::to_string(score) + 
                              "\n\nPress R to restart\nPress ESC for menu\nPress Q to quit");
    
    // Center the instruction text
    sf::FloatRect textRect = instructionText.getLocalBounds();
    instructionText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    instructionText.setPosition(400, 300);
    
    // Draw title and instructions
    window.draw(titleText);
    window.draw(instructionText);
    
    // Reset title text color for next time
    titleText.setFillColor(sf::Color::Green);
}

void Game::checkCollisions() {
    // Check vehicle collisions
    for (const auto& vehicle : vehicles) {
        if (player->getBounds().intersects(vehicle->getBounds())) {
            player->loseLife();
            return;
        }
    }
    
    // Check water collisions
    if (player->getPosition().y > 150 && player->getPosition().y < 350) {
        bool onSafePlatform = false;
        
        // Check if player is on a log
        for (const auto& log : logs) {
            if (player->getBounds().intersects(log->getBounds())) {
                onSafePlatform = true;
                player->setOnLog(true);
                player->moveWithLog(log->getMovement(0.016f)); // Approximate deltaTime
                break;
            }
        }
        
        // Check if player is on a crocodile
        if (!onSafePlatform) {
            for (const auto& croc : crocodiles) {
                if (player->getBounds().intersects(croc->getBounds())) {
                    // If the crocodile is dangerous (mouth open), the player loses a life
                    if (croc->isDangerous()) {
                        player->loseLife();
                        return;
                    } else {
                        // Otherwise, the player can ride the crocodile
                        onSafePlatform = true;
                        player->setOnLog(true);  // Reusing log movement mechanism
                        player->moveWithLog(croc->getMovement(0.016f));
                        break;
                    }
                }
            }
        }
        
        // If in water but not on a log or safe crocodile, lose a life
        if (!onSafePlatform) {
            player->setOnLog(false);
            player->loseLife();
        }
    } else {
        player->setOnLog(false);
    }
}

void Game::spawnVehicle() {
    // Random y position on the road area
    std::uniform_int_distribution<int> yDist(350, 550);
    int y = yDist(rng);
    
    // Random speed
    std::uniform_real_distribution<float> speedDist(100.0f, 300.0f);
    float speed = speedDist(rng);
    
    // Random direction (left or right)
    bool goingRight = std::uniform_int_distribution<int>(0, 1)(rng) == 1;
    sf::Vector2f direction = goingRight ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
    
    // Spawn position depends on direction
    float x = goingRight ? -50.0f : 850.0f;
    
    vehicles.push_back(std::make_unique<Vehicle>(x, y, speed, direction));
}

void Game::spawnLog() {
    // Random y position on the water area - more distributed
    std::uniform_int_distribution<int> yDist(160, 340);
    int y = yDist(rng);
    
    // Random speed
    std::uniform_real_distribution<float> speedDist(50.0f, 150.0f);
    float speed = speedDist(rng);
    
    // Random direction (left or right)
    bool goingRight = std::uniform_int_distribution<int>(0, 1)(rng) == 1;
    sf::Vector2f direction = goingRight ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
    
    // Spawn position depends on direction
    float x = goingRight ? -100.0f : 900.0f;
    
    logs.push_back(std::make_unique<Log>(x, y, speed, direction));
}

void Game::spawnCrocodile() {
    // Random y position on the water area
    std::uniform_int_distribution<int> yDist(150, 350);
    int y = yDist(rng);
    
    // Random speed
    std::uniform_real_distribution<float> speedDist(40.0f, 100.0f);
    float speed = speedDist(rng);
    
    // Random direction (left or right)
    bool goingRight = std::uniform_int_distribution<int>(0, 1)(rng) == 1;
    sf::Vector2f direction = goingRight ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
    
    // Spawn position depends on direction
    float x = goingRight ? -100.0f : 900.0f;
    
    crocodiles.push_back(std::make_unique<Crocodile>(x, y, speed, direction));
}

void Game::resetGame() {
    score = 0;
    logSpawnTimer = 0;
    vehicleSpawnTimer = 0;
    crocodileSpawnTimer = 0;
    
    // Clear entities
    vehicles.clear();
    logs.clear();
    crocodiles.clear();
    
    // Reset player
    player = std::make_unique<Player>(400, 550);
    
    // Reset title text in case it was changed
    titleText.setFillColor(sf::Color::Green);
    
    // Set up initial state with some vehicles and logs
    for (int i = 0; i < 3; i++) {
        // Create a few vehicles
        float y = 350 + (i * 50);
        float speed = 100.0f + (i * 50.0f);
        bool goingRight = (i % 2) == 0;
        sf::Vector2f direction = goingRight ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
        float x = goingRight ? -50.0f : 850.0f;
        
        vehicles.push_back(std::make_unique<Vehicle>(x, y, speed, direction));
    }
    
    // Create several logs spread across water area
    for (int i = 0; i < 5; i++) {
        float y = 170 + (i * 35);  // More evenly distributed logs
        float speed = 50.0f + (i * 15.0f);
        bool goingRight = (i % 2) == 1;
        sf::Vector2f direction = goingRight ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
        float x = goingRight ? -100.0f : 900.0f;
        
        logs.push_back(std::make_unique<Log>(x, y, speed, direction));
    }
    
    // Create a couple of crocodiles
    for (int i = 0; i < 2; i++) {
        float y = 200 + (i * 70);
        float speed = 40.0f + (i * 20.0f);
        bool goingRight = (i % 2) == 0;
        sf::Vector2f direction = goingRight ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
        float x = goingRight ? -100.0f : 900.0f;
        
        crocodiles.push_back(std::make_unique<Crocodile>(x, y, speed, direction));
    }
}