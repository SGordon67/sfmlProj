#include <iostream>

#include "BackgroundObject.h"

BackgroundObject::BackgroundObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename)
	: BasicObject(position, size, rotation, renderLayer, filename)
{
	std::cout << "BackgroundObject constructed" << std::endl;
}
BackgroundObject::BackgroundObject(const BackgroundObject& other) // copy constructor
	: BasicObject(other)
{
}
BackgroundObject::BackgroundObject(BackgroundObject&& other) noexcept // move constructor
	: BasicObject(other)
{
}
