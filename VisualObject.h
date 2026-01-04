#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include <cmath>
#include <random>

#include "enums.h"
#include "BasicObject.h"

class VisualObject : public BasicObject
{
protected:
	RenderLayer m_renderLayer;
	sf::Texture m_texture;
	sf::IntRect m_spriteRect;
	sf::Sprite m_sprite;

public:
	VisualObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename);
	VisualObject(const VisualObject& other); // copy constructor
	VisualObject(const VisualObject&& other) noexcept; // move constructor

	RenderLayer getRenderLayer() const;
	sf::Texture getTexture() const;
	sf::IntRect getSpriteRect() const;
	sf::Sprite getSprite() const;

	virtual void setPosition(sf::Vector2f position) override;

	void loadTexture(const std::string& filename);

	virtual void rotate(const float rotation);
	virtual void update(float deltaTime, sf::Vector2f relationalVelocity);
};

#endif
