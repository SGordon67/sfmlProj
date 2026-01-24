#include "globals.h"
#include "SFML/Graphics/Texture.hpp"
#include <iostream>

int numObjects = 0;

// needs to be a multiple of the background size (1500 x 750) (I could alter it if I wasn't lazy)
float worldWidth = 3000; // usually 3000
float worldHeight = 1500; // usually 1500

int windowWidth = 1500;
int windowHeight = 750;
int frameRateLimit = 60;
sf::Vector2i windowPos = {1930, 10};

int viewWidth = windowWidth;
int viewHeight = windowHeight;

// keys
bool escPressed = false;
bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool interactPressed = false;
bool tabPressed = false;
bool* buttons[numButtons];

sf::Texture farBackgroundTexture;
sf::Texture closeBackgroundTexture;
sf::Texture meteorTexture;
sf::Texture crateTexture;
sf::Texture spikeyTexture;
sf::Texture playerTexture;
sf::Texture enemyTexture;
