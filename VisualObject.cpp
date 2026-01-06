#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include <iostream>
#include <cmath>

#include "VisualObject.h"

VisualObject::VisualObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename)
	: BasicObject(position, size, rotation)
	, m_renderLayer(renderLayer)
	, m_spriteRect(sf::Vector2i{0, 0}, size)
	, m_sprite(m_texture, m_spriteRect)
{
	loadTexture(filename);
	m_sprite.setPosition(m_position);
	// std::cout << "VisualObject constructed" << std::endl;
}
VisualObject::VisualObject(const VisualObject& other) // copy constructor
	: BasicObject(other.m_position, other.m_size, other.m_rotation)
	, m_renderLayer(other.m_renderLayer)
	, m_spriteRect(other.m_spriteRect)
	, m_texture(other.m_texture)
		, m_sprite(other.m_sprite)
{
	std::cout << "Copy constructor called for visual object" << std::endl;
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
}
VisualObject::VisualObject(const VisualObject&& other) noexcept // move constructor
	: BasicObject(other.m_position, other.m_size, other.m_rotation)
	, m_renderLayer(other.m_renderLayer)
	, m_spriteRect(other.m_spriteRect)
	, m_texture(std::move(other.m_texture))
, m_sprite(other.m_sprite)
{
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
}

RenderLayer VisualObject::getRenderLayer() const
{
	return m_renderLayer;
}

sf::Texture VisualObject::getTexture() const
{
	return m_texture;
}

sf::IntRect VisualObject::getSpriteRect() const
{
	return m_spriteRect;
}

sf::Sprite VisualObject::getSprite() const
{
	return m_sprite;
}

void VisualObject::setPosition(sf::Vector2f position)
{
	m_position = position;
	m_sprite.setPosition(m_position);
}

void VisualObject::loadTexture(const std::string& filename)
{
	if(!m_texture.loadFromFile(filename))
		std::cout << "Sprite not loaded :(" << std::endl;
	m_texture.setRepeated(true);
	m_sprite.setTexture(m_texture);
	std::cout << "Texture loaded from file :) " << std::endl;
}

void VisualObject::rotate(const float rotation)
{
	// std::cout << "Rotation object by: " << rotation << " degrees" << std::endl;
	m_sprite.rotate(sf::degrees(rotation));
	m_rotation -= rotation;
	if(m_rotation >= 360) m_rotation -= 360;
	if(m_rotation < 0) m_rotation += 360;
}

void VisualObject::update(float deltaTime, sf::Vector2f relationalVelocity)
{
	// move the background at different rates in relation to the player for a feeling of depth
	switch(m_renderLayer)
	{
		case RenderLayer::FarBackground:
			// std::cout << "Moving the far background" << std::endl;
			m_sprite.move(0.5f * relationalVelocity * deltaTime);
			break;
		case RenderLayer::CloseBackground:
			// std::cout << "Moving the close background" << std::endl;
			m_sprite.move(0.8f * relationalVelocity * deltaTime);
			break;
		case RenderLayer::Main:
			break;
		case RenderLayer::Foreground:
			// m_sprite.move(1.3f * relationalVelocity * deltaTime);
			break;
	}
}
