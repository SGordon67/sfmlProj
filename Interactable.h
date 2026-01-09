#include <iostream>
#include <string>

#include "Player.h"

class Player;

class Interactable
{
public:
	virtual ~Interactable() = default;

	virtual void interact(Player& player) = 0;

	virtual float getInteractionRadius() const
	{
		return 50.0f;
	}

	virtual bool canInteract(const Player& player) const
	{
		std::cout << "Determining if can interact with player: " << player.getObjectID() << std::endl;
		return true;
	}

	virtual std::string getInteractionPrompt() const
	{
		return "Press E to interact";
	}
};
