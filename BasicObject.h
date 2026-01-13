#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <cmath>
#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics.hpp"

#include "globals.h"
#include "enums.h"

extern std::vector<sf::Vector2f> getDupPositions(sf::Vector2f position, sf::Vector2i size);
extern float radiansToDegrees(float radians);

// forward declaration, this class will exist
class PhysicalObject;

class BasicObject
{
protected:
	int m_objectID;
	sf::Vector2f m_position;
	sf::Vector2i m_size;
	float m_rotation;

	RenderLayer m_renderLayer;
	sf::Texture m_texture;
	sf::IntRect m_spriteRect;
	sf::Sprite m_sprite;
public:

	BasicObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename)
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
				  m_sprite.setOrigin({size.x/2.f, size.y/2.f});
				  m_sprite.setPosition(m_position);
			  }
	BasicObject(const BasicObject& other) // copy constructor
			: m_objectID(++numObjects)
			  , m_position(other.m_position)
			  , m_size(other.m_size)
			  , m_rotation(other.m_rotation)
			  , m_renderLayer(other.m_renderLayer)
			  , m_texture(other.m_texture)
			  , m_spriteRect(other.m_spriteRect)
			  , m_sprite(other.m_sprite)
	{
		std::cout << "Copy constructor called for basic object" << std::endl;
		m_sprite.setTexture(m_texture);
		m_sprite.setOrigin({m_size.x/2.f, m_size.y/2.f});
		m_sprite.setPosition(m_position);
	}
	BasicObject(BasicObject&& other) noexcept // move constructor
			: m_objectID(other.m_objectID)
			, m_position(other.m_position)
			, m_size(other.m_size)
			, m_rotation(other.m_rotation)
			, m_renderLayer(other.m_renderLayer)
			, m_texture(std::move(other.m_texture))
			, m_spriteRect(other.m_spriteRect)
			, m_sprite(std::move(other.m_sprite))
			{
				std::cout << "Move constructor called for basic object" << std::endl;
				m_sprite.setTexture(m_texture);
				m_sprite.setOrigin({m_size.x/2.f, m_size.y/2.f});
				m_sprite.setPosition(m_position);
			}

	int getObjectID() const
	{
		return m_objectID;
	}

	sf::Vector2f getPosition() const
	{
		return m_position;
	}
	void setPosition(sf::Vector2f position)
	{
		m_position = position;
		m_sprite.setPosition(position);
	}

	sf::Vector2i getSize() const
	{
		return m_size;
	}
	void setSize(sf::Vector2i size)
	{
		m_size = size;
	}

	float getRotation() const
	{
		return m_rotation;
	}
	void setRotation(float rotation)
	{
		m_rotation = rotation;
	}

	RenderLayer getRenderLayer() const
	{
		return m_renderLayer;
	}
	void setRenderLayer(RenderLayer layer)
	{
		m_renderLayer = layer;
	}

	sf::Texture getTexture() const
	{
		return m_texture;
	}

	sf::IntRect getSpriteRect() const
	{
		return m_spriteRect;
	}

	sf::Sprite getSprite() const
	{
		return m_sprite;
	}

	void loadTexture(const std::string& filename)
	{
		if(!m_texture.loadFromFile(filename))
		{
			std::cout << "Sprite not loaded :(" << std::endl;
		}
		m_texture.setRepeated(true);
		m_sprite.setTexture(m_texture);
		// std::cout << "Texture loaded from file :) " << std::endl;
	}

	void rotate(const float rotation)
	{
		// std::cout << "Rotation object by: " << rotation << " degrees" << std::endl;
		m_sprite.rotate(sf::degrees(radiansToDegrees(rotation)));
		m_rotation += rotation;
		if(m_rotation >= 2 * M_PI) m_rotation -= 2 * M_PI;
		if(m_rotation < 0) m_rotation += 2 * M_PI;
	}

	void basicUpdate(float deltaTime, sf::Vector2f relationalVelocity)
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

	// this function draws the object on the window, and it's duplicates for edge wrapping
	// it also returns the extra coords to the calling function if more is needed to be drawn in subclasses
	std::vector<sf::Vector2f> basicDraw(sf::RenderWindow& window)
	{
		// main object
		window.draw(m_sprite);

		// vector of positions to return
		std::vector<sf::Vector2f> dupPositions;
		if(m_renderLayer != RenderLayer::Main) return dupPositions;

		// get the duplicates needed for edge wrap
		dupPositions = getDupPositions(m_position, m_size);

		sf::Vector2f originalPosition = m_position;
		for(const auto& pos : dupPositions)
		{
			setPosition(pos);
			window.draw(m_sprite);
		}
		setPosition(originalPosition);

		return dupPositions;
	}
};

#endif
