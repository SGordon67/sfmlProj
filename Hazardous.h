#include <iostream>
#include <string>

#include "Player.h"

class Hazardous
{
public:
	virtual ~Hazardous() = default;

	virtual void dealDamage(Player& player) = 0;

	virtual sf::Vector2f getPosition() const
	{
		return sf::Vector2f(0.f, 0.f);
	}

	virtual float getRadius() const
	{
		return 50.0f;
	}

	virtual bool canInteract() const
	{
		std::cout << "Determining if object can be interacted with." << std::endl;
		return true;
	}

	virtual std::string getInteractionPrompt() const
	{
		return "Press E to interact";
	}
};
