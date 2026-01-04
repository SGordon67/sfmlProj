#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include "SFML/System/Vector2.hpp"

class BasicObject
{
protected:
	int m_objectID;
	sf::Vector2f m_position;
	sf::Vector2i m_size;
	float m_rotation;
public:
	BasicObject(sf::Vector2f position, sf::Vector2i size, float rotation);

	sf::Vector2f getPosition() const;
	float getRotation() const;

	virtual void setPosition(sf::Vector2f position);
};

#endif
