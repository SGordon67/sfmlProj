#include "globals.h"
#include "SFML/Graphics/Texture.hpp"

int numObjects = 0;

// changed bg image to (1000 x 1000) for both background are foreground
float worldWidth = 3000;
float worldHeight = 3000;

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
