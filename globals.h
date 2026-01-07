#ifndef GLOBALS_H
#define GLOBALS_H

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"

constexpr bool showHitboxes = true;
constexpr int numButtons = 5;
constexpr sf::Color hitboxColor = sf::Color(255, 0, 0, 50);
constexpr float FixedDeltaTime = 1.0f / 60.0f;
constexpr float restitution = 0.8f;

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
