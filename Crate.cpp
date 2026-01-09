#include <iostream>

#include "Crate.h"
#include "Player.h"
#include "VisualObject.h"

Crate::Crate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float interactRadius)
	: VisualObject(position, size, rotation, renderLayer, filename)
	, m_interactRadius(interactRadius)
{
}
Crate::Crate(const Crate& other) // copy constructor
	: VisualObject(other)
	, m_interactRadius(other.m_interactRadius)
{
}
Crate::Crate(Crate&& other) noexcept // move constructor
	: VisualObject(std::move(other))
	, m_interactRadius(other.m_interactRadius)
{
}

void Crate::interact(Player& player)
{
	std::cout << "Player " << player.getObjectID() << " interacting with object: " << m_objectID << std::endl;
}
