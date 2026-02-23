#ifndef IMAGE_H
#define IMAGE_H

#include "VisualObject.h"

class Image : public VisualObject
{
protected:

public:
	Image(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture);
	Image(const Image& other); // copy constructor
	Image(Image&& other) noexcept; // move constructor

	virtual void visualUpdate(float deltaTime, sf::Vector2f relationalVelocity);
    virtual void draw(sf::RenderWindow& window);
};

#endif
