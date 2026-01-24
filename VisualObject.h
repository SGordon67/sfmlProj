#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "BasicObject.h"

class VisualObject : public BasicObject
{
protected:

public:
	VisualObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture);
	VisualObject(const VisualObject& other); // copy constructor
	VisualObject(VisualObject&& other) noexcept; // move constructor

	virtual void visualUpdate(float deltaTime, sf::Vector2f relationalVelocity);
};

#endif
