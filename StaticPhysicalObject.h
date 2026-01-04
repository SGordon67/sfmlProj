#ifndef STATICPHYSICALOBJECT_H
#define STATICPHYSICALOBJECT_H

#include "VisualObject.h"

class StaticPhysicalObject : public VisualObject
{
protected:
public:
	StaticPhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename);
};

#endif
