#include "SFML/System/Vector2.hpp"

#include "BasicObject.h"

extern int numObjects;

BasicObject::BasicObject(sf::Vector2f position, sf::Vector2i size, float rotation)
	: m_objectID(++numObjects)
	, m_position(position)
	, m_size(size)
	, m_rotation(rotation)
{
	// std::cout << "BasicObject constructed" << std::endl;
}

sf::Vector2f BasicObject::getPosition() const
{
	return m_position;
}
float BasicObject::getRotation() const
{
	return m_rotation;
}

void BasicObject::setPosition(sf::Vector2f position)
{
	m_position = position;
}
