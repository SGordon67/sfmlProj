#ifndef BACKGROUNDOBJECT_H
#define BACKGROUNDOBJECT_H

#include "BasicObject.h"

class BackgroundObject : public BasicObject
{
protected:

public:
	BackgroundObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename);
	BackgroundObject(const BackgroundObject& other); // copy constructor
	BackgroundObject(BackgroundObject&& other) noexcept; // move constructor
};

#endif
