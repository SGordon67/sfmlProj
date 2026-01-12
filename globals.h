#ifndef GLOBALS_H
#define GLOBALS_H

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"

// debug
constexpr bool showHitboxes = true;

// structural values
constexpr int numButtons = 7;
constexpr float FixedDeltaTime = 1.0f / 60.0f;

// globals consts
constexpr sf::Color hitboxColor = sf::Color(255, 0, 0, 150);

constexpr float restitution = 1.f;
constexpr float friction = 0.5f;
constexpr float dragCoef = 4;
constexpr float angularDrag = 0.99f;

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

#endif
