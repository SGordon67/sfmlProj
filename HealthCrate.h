#ifndef HEALTHCRATE_H
#define HEALTHCRATE_H

#include "Crate.h"

class HealthCrate : public Crate
{
protected:
    int m_health;
public:
	HealthCrate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
		float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
		float interactRadius, bool used, int value);
	HealthCrate(const HealthCrate& other); // copy constructor
	HealthCrate(HealthCrate&& other) noexcept; // move constructor

    int getValue();
    void setValue(int value);

    // Interactable
    int getObjectID() const override;
	sf::Vector2f getPosition() const override;
	bool canInteract() const override;
	float getInteractionRadius() const override;
	void interact(Player& player) override;
};

#endif
