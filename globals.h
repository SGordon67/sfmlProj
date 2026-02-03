#ifndef GLOBALS_H
#define GLOBALS_H

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

// debug
constexpr bool showHitboxes = true;

// structural values
constexpr int numButtons = 7;
constexpr float FixedDeltaTime = 1.0f / 60.0f;

// globals consts
constexpr float restitution = 1.f;
constexpr float friction = 0.5f;
constexpr float dragCoef = 4;
constexpr float angularDrag = 0.99f;
constexpr sf::Color hitboxColor = sf::Color(255, 0, 0, 150);

// global variables
extern int numObjects;

extern float worldWidth;
extern float worldHeight;

extern int windowWidth;
extern int windowHeight;
extern int frameRateLimit;
extern sf::Vector2i windowPos;

extern int viewWidth;
extern int viewHeight;

// minimap
extern float minimapPositionXFraction;
extern float minimapPositionYFraction;
extern float minimapWidthFraction;
extern float minimapHeightFraction;

// keys
extern bool escPressed;
extern bool upPressed;
extern bool downPressed;
extern bool leftPressed;
extern bool rightPressed;
extern bool interactPressed;
extern bool tabPressed;
extern bool* buttons[numButtons];

// textures
extern sf::Texture farBackgroundTexture;
extern sf::Texture closeBackgroundTexture;
extern sf::Texture meteorTexture;
extern sf::Texture crateTexture;
extern sf::Texture spikeyTexture;
extern sf::Texture playerTexture;
extern sf::Texture enemyTexture;

#endif
