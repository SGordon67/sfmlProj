#ifndef CRATE_H
#define CRATE_H

#include "VisualObject.h"
#include "Interactable.h"

class Crate : public VisualObject, public Interactable
{
protected:
	float m_interactRadius;
public:
	Crate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float interactRadius);
	Crate(const Crate& other); // copy constructor
	Crate(Crate&& other) noexcept; // move constructor

	void interact(Player& player);
};

#endif
