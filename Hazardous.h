#ifndef HAZARDOUS_H
#define HAZARDOUS_H

#include "Entity.h"

class Hazardous
{
public:
	virtual ~Hazardous() {}

    virtual int getObjectID() const = 0;
	virtual sf::Vector2f getPosition() const = 0;
	virtual float getRadius() const = 0;
	virtual void dealDamage(Entity& Entity) = 0;
    virtual void update() = 0;
};

#endif
