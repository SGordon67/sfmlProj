#ifndef GLOBALS_H
#define GLOBALS_H

#include "SFML/System/Vector2.hpp"

constexpr int numButtons = 5;


extern int numObjects;

extern int windowWidth;
extern int windowHeight;
extern int frameRateLimit;
extern sf::Vector2i windowPos;

extern int viewWidth;
extern int viewHeight;

extern const float FixedDeltaTime;

#endif
