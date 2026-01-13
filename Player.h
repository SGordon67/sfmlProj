#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
private:
	float m_angularAcceleration;
public:
	Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, int hp, int maxHP, float angularAccleration);

	float getAngularAcceleration();
	void setAngularAcceleration(float angularAcceleration);

	void updateRotation() override;
	void printInfo();
	void playerUpdate(bool* (&buttons)[numButtons]);
};

#endif
