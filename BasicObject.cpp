#include <iostream>

#include "BasicObject.h"

BasicObject::BasicObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename)
	: m_objectID(++numObjects)
	, m_position(position)
	, m_size(size)
	, m_rotation(rotation)
	, m_renderLayer(renderLayer)
	, m_spriteRect(sf::Vector2i{0, 0}, size)
	, m_sprite(m_texture, m_spriteRect)
{
	std::cout << "Regular constructor called for basic object" << std::endl;
	loadTexture(filename);
	m_sprite.setPosition(m_position);
}
BasicObject::BasicObject(const BasicObject& other) // copy constructor
	: m_objectID(++numObjects)
	, m_position(other.m_position)
	, m_size(other.m_size)
	, m_rotation(other.m_rotation)
	, m_renderLayer(other.m_renderLayer)
	, m_spriteRect(other.m_spriteRect)
	, m_texture(other.m_texture)
	, m_sprite(other.m_sprite)
{
	std::cout << "Copy constructor called for basic object" << std::endl;
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
}
BasicObject::BasicObject(BasicObject&& other) noexcept // move constructor
	: m_objectID(other.m_objectID)
	, m_position(other.m_position)
	, m_size(other.m_size)
	, m_rotation(other.m_rotation)
	, m_renderLayer(other.m_renderLayer)
	, m_spriteRect(other.m_spriteRect)
	, m_texture(std::move(other.m_texture))
	, m_sprite(other.m_sprite)
{
	std::cout << "Move constructor called for basic object" << std::endl;
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
}

int BasicObject::getObjectID() const
{
	return m_objectID;
}

sf::Vector2f BasicObject::getPosition() const
{
	return m_position;
}

sf::Vector2i BasicObject::getSize() const
{
	return m_size;
}

float BasicObject::getRotation() const
{
	return m_rotation;
}

RenderLayer BasicObject::getRenderLayer() const
{
	return m_renderLayer;
}

sf::Texture BasicObject::getTexture() const
{
	return m_texture;
}

sf::IntRect BasicObject::getSpriteRect() const
{
	return m_spriteRect;
}

sf::Sprite BasicObject::getSprite() const
{
	return m_sprite;
}

void BasicObject::setPosition(sf::Vector2f position)
{
	m_position = position;
	m_sprite.setPosition(m_position);
}

void BasicObject::loadTexture(const std::string& filename)
{
	if(!m_texture.loadFromFile(filename))
	{
		std::cout << "Sprite not loaded :(" << std::endl;
	}
	m_texture.setRepeated(true);
	m_sprite.setTexture(m_texture);
	// std::cout << "Texture loaded from file :) " << std::endl;
}

void BasicObject::rotate(const float rotation)
{
	// std::cout << "Rotation object by: " << rotation << " degrees" << std::endl;
	m_sprite.rotate(sf::degrees(rotation));
	m_rotation -= rotation;
	if(m_rotation >= 360) m_rotation -= 360;
	if(m_rotation < 0) m_rotation += 360;
}

void BasicObject::update(float deltaTime, sf::Vector2f relationalVelocity)
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
