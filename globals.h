#ifndef GLOBALS_H
#define GLOBALS_H

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

// debug
constexpr bool showHitboxes = false;

// structural values
constexpr float FixedDeltaTime = 1.0f / 60.0f;

// globals consts
constexpr float restitution = 0.9f;
constexpr float friction = 0.5f;
constexpr float dragCoef = 4;
constexpr float angularDrag = 0.98f;
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

// textures
extern sf::Texture farBackgroundTexture;
extern sf::Texture closeBackgroundTexture;
extern sf::Texture meteorTexture;
extern sf::Texture crateTexture;
extern sf::Texture spikeyTexture;
extern sf::Texture playerTexture;
extern sf::Texture enemyTexture;

#endif
