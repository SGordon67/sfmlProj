#include <iostream>

#include "Crate.h"
#include "BasicObject.h"
#include "Player.h"
#include "SFML/System/Vector2.hpp"
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

sf::Vector2f Crate::getPosition() const
{
	return BasicObject::getPosition();
}

bool Crate::canInteract() const
{
	return !m_used;
}

float Crate::getInteractionRadius() const
{
	return m_interactRadius;
}

void Crate::interact(Player& player)
{
	m_used = true;
	std::cout << "Player ID (" << player.getObjectID() << ") interacting with object (" << m_objectID << ")" << std::endl;
}

std::string Crate::getInteractionPrompt() const
{
	return "press E to interact";
}
