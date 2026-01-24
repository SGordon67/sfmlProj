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
void initializeButtons()
{
    buttons[0] = &escPressed;
    buttons[1] = &upPressed;
    buttons[2] = &downPressed;
    buttons[3] = &leftPressed;
    buttons[4] = &rightPressed;
    buttons[5] = &interactPressed;
    buttons[6] = &tabPressed;
}

sf::Texture farBackgroundTexture;
sf::Texture closeBackgroundTexture;
sf::Texture meteorTexture;
sf::Texture crateTexture;
sf::Texture spikeyTexture;
sf::Texture playerTexture;
sf::Texture enemyTexture;
void initializeTextures()
{
    if(!farBackgroundTexture.loadFromFile("art/basicBackground.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    farBackgroundTexture.setRepeated(true);
    if(!closeBackgroundTexture.loadFromFile("art/basicStars.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    closeBackgroundTexture.setRepeated(true);
    if(!meteorTexture.loadFromFile("art/basicMeteor.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!crateTexture.loadFromFile("art/basicCrate.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!spikeyTexture.loadFromFile("art/basicSpikey.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!playerTexture.loadFromFile("art/basicSpriteL.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!enemyTexture.loadFromFile("art/basicEnemy.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
}
