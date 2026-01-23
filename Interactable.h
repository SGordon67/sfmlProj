#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "Player.h"
#include <memory>

class Interactable
{
public:
	virtual ~Interactable() {}
    virtual int getObjectID() const = 0;
	virtual sf::Vector2f getPosition() const = 0;
	virtual float getInteractionRadius() const
	{
		return 50.0f;
	}
	virtual bool canInteract() const = 0;
	virtual void interact(std::shared_ptr<Player> player) = 0;
};

#endif
