#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics.hpp"

#include "globals.h"
#include "enums.h"

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
	BasicObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename);
	BasicObject(const BasicObject& other); // copy constructor
	BasicObject(BasicObject&& other) noexcept; // move constructor

	int getObjectID() const;
	sf::Vector2f getPosition() const;
	sf::Vector2i getSize() const;
	float getRotation() const;
	RenderLayer getRenderLayer() const;
	sf::Texture getTexture() const;
	sf::IntRect getSpriteRect() const;
	sf::Sprite getSprite() const;

	virtual void setPosition(sf::Vector2f position);

	void loadTexture(const std::string& filename);

	virtual void rotate(const float rotation);
	virtual void basicUpdate(float deltaTime, sf::Vector2f relationalVelocity);

	virtual std::vector<sf::Vector2f> basicDraw(sf::RenderWindow& window);
};

#endif
