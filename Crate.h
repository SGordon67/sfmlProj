#ifndef CRATE_H
#define CRATE_H

#include "VisualObject.h"
#include "Interactable.h"

class Crate : public VisualObject, public Interactable
{
protected:
	float m_interactRadius;
	bool m_used;
public:
	Crate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float interactRadius);
	Crate(const Crate& other); // copy constructor
	Crate(Crate&& other) noexcept; // move constructor

	sf::Vector2f getPosition() const override;

	bool canInteract() const override;
	float getInteractionRadius() const override;

	void interact(Player& player) override;
	std::string getInteractionPrompt() const override;
};

#endif
