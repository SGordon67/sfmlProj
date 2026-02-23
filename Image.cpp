#include "Image.h"
#include "VisualObject.h"

Image::Image(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture)
	: VisualObject(position, size, rotation, renderLayer, texture)
{
}
Image::Image(const Image& other) // copy constructor
	: VisualObject(other)
{
}
Image::Image(Image&& other) noexcept // move constructor
	: VisualObject(std::move(other))
{
}
void Image::visualUpdate(float deltaTime, sf::Vector2f relationalVelocity)
{
    VisualObject::basicUpdate(deltaTime, relationalVelocity);
}
void Image::draw(sf::RenderWindow& window)
{
    VisualObject::basicDraw(window);
}
