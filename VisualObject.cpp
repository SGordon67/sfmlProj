#include "VisualObject.h"
#include "BasicObject.h"

VisualObject::VisualObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture)
	: BasicObject(position, size, rotation, renderLayer, texture)
{
}
VisualObject::VisualObject(const VisualObject& other) // copy constructor
	: BasicObject(other)
{
}
VisualObject::VisualObject(VisualObject&& other) noexcept // move constructor
	: BasicObject(std::move(other))
{
}
void VisualObject::visualUpdate(float deltaTime, sf::Vector2f relationalVelocity)
{
	basicUpdate(deltaTime, relationalVelocity);
}
