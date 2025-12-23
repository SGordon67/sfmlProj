#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>

const int numButtons = 5;

class Player
{
	sf::Vector2i m_size;
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	float m_rotation;

	float m_mSpeed;

	float m_acceleration;
	float m_drag;
	float m_rotSpeed;

	sf::Texture m_texture;
	sf::IntRect m_spriteRect;
	sf::Sprite m_sprite;

public:

	Player(sf::Vector2i size, sf::Vector2f position, sf::Vector2f speed, float rotation, float maxSpeed, float acceleration, float drag, float rotationSpeed, const sf::Texture& texture)
			: m_size(size)
			  , m_position(position)
			  , m_speed(speed)
			  , m_rotation(rotation)
			  , m_mSpeed(maxSpeed)
			  , m_acceleration(acceleration)
			  , m_drag(drag)
			  , m_rotSpeed(rotationSpeed)
			  , m_texture(texture)
			  , m_spriteRect(sf::Vector2i{0, 0},size)
			  , m_sprite(m_texture, m_spriteRect)
			  {
				  m_sprite.setOrigin({21, 25});
				  std::cout << "Player Constructed" << std::endl;
			  }

	sf::Vector2f getPosition() const
	{
		return m_position;
	}
	sf::Vector2f getSpeed() const
	{
		return m_speed;
	}
	float getRotation() const
	{
		return m_rotation;
	}

	float getMaxSpeed() const
	{
		return m_mSpeed;
	}

	float getAcceleration() const
	{
		return m_acceleration;
	}
	float getdrag() const
	{
		return m_drag;
	}
	float getRotSpeed() const
	{
		return m_rotSpeed;
	}

	sf::Sprite getSprite()
	{
		return m_sprite;
	}

	void updatePosition(int accelerate)
	{
		// zero out the speed if its small enough
		if(m_speed.x < 0.005 && m_speed.x > -0.005) m_speed.x = 0;
		if(m_speed.y < 0.005 && m_speed.y > -0.005) m_speed.y = 0;

		// force of drag
		sf::Vector2f fDrag = {-m_drag * m_speed.x, -m_drag * m_speed.y};
		if(m_speed.x == 0) fDrag.x = 0;
		if(m_speed.y == 0) fDrag.y = 0;

		// get the angle the player is facing
		float forward = m_rotation;
		forward = forward * (M_PI / 180); // convert to radians

		// force of acceleration
		sf::Vector2f fAccel = {0, 0};
		if(accelerate == 1)
		{
			fAccel = {m_acceleration * std::cos(forward), -m_acceleration * std::sin(forward)};
		}
		else if(accelerate == -1)
		{
			fAccel = {-m_acceleration * std::cos(forward), m_acceleration * std::sin(forward)};
		}

		// range of possible speeds to stay inbetween
		float maxXSpeed = m_mSpeed * std::cos(forward);
		float maxYSpeed = m_mSpeed * std::sin(forward);
		float minXSpeed = -m_mSpeed * std::cos(forward);
		float minYSpeed = -m_mSpeed * std::sin(forward);

		std::cout << "fDrag: " << fDrag.x << ", " << fDrag.y << std::endl;
		std::cout << "fAccel: " << fAccel.x << ", " << fAccel.y << std::endl;
		std::cout << "Current Speed: " << m_speed.x << ", " << m_speed.y << std::endl;
		std::cout << "Current Position: " << m_position.x << ", " << m_position.y << std::endl;

		// add the forces
		m_speed += fDrag + fAccel;
		
		// make sure we're not going too fast
		// max/mins are based on quadrant
		float currentDirection = std::atan(m_speed.y / m_speed.x);
		if(forward <= M_PI/2) // Q1
		{
			if(m_speed.x > maxXSpeed || m_speed.x < minXSpeed)
			{
				m_speed.x -= fAccel.x;
			}
			if(m_speed.y > maxYSpeed || m_speed.y < minYSpeed)
			{
				m_speed.y -= fAccel.y;
			}
		}
		else if(forward <= M_PI) // Q2
		{
			if(m_speed.x < maxXSpeed || m_speed.x > minXSpeed)
			{
				m_speed.x -= fAccel.x;
			}
			if(m_speed.y > maxYSpeed || m_speed.y < minYSpeed)
			{
				m_speed.y -= fAccel.y;
			}
		}
		else if(forward <= 3 * M_PI / 2) // Q3
		{
			if(m_speed.x < maxXSpeed || m_speed.x > minXSpeed)
			{
				m_speed.x -= fAccel.x;
			}
			if(m_speed.y < maxYSpeed || m_speed.y > minYSpeed)
			{
				m_speed.y -= fAccel.y;
			}
		}
		else // Q4
		{
			if(m_speed.x > maxXSpeed || m_speed.x < minXSpeed)
			{
				m_speed.x -= fAccel.x;
			}
			if(m_speed.y < maxYSpeed || m_speed.y > minYSpeed)
			{
				m_speed.y -= fAccel.y;
			}
		}

		// update the position
		m_position += m_speed;
		m_sprite.setPosition({m_position.x, m_position.y});
		return;
	}
	void rotate(float rotVal)
	{
		// std::cout << "Rotating player by rotVal: " << rotVal << std::endl;
		m_sprite.rotate(sf::degrees(rotVal));
		m_rotation -= rotVal;
		if(m_rotation >= 360) m_rotation -= 360;
		if(m_rotation < 0) m_rotation += 360;
		return;
	}
};

class Arena
{
	sf::Vector2i m_size;

public:

	Arena(sf::Vector2i size)
			: m_size(size)
	{
	}
};

void updateButtonPresses(bool* (&buttons)[numButtons])
{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			// std::cout << "Key Pressed: Escape" << std::endl;
			*buttons[0] = true;
		}else
		{
			*buttons[0] = false;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			// std::cout << "Key Pressed: W/Up" << std::endl;
			*buttons[1] = true;
		}else
		{
			*buttons[1] = false;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			// std::cout << "Key Pressed: S/Down" << std::endl;
			*buttons[2] = true;
		}else 
		{
			*buttons[2] = false;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			// std::cout << "Key Pressed: A/Left" << std::endl;
			*buttons[3] = true;
		}else
		{
			*buttons[3] = false;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			// std::cout << "Key Pressed: D/Right" << std::endl;
			*buttons[4] = true;
		}else
		{
			*buttons[4] = false;
		}
}

void updateGame(sf::Time& elapsed, sf::RenderWindow& window, bool* (&buttons)[numButtons], Player& p, Arena& a)
{
	// handle acceleration
	sf::Vector2f curSpeed = p.getSpeed();
	int accel = 0;
	if(*buttons[1] && !*buttons[2])
	{
		// std::cout << "Accelerating" << std::endl;
		accel = 1;
	}
	else if(*buttons[2] && !*buttons[1])
	{
		// std::cout << "decelerating" << std::endl;
		accel = -1;
	}
	p.updatePosition(accel);
	


	// handle rotation
	float potentialRotation = 0;
	if(*buttons[3]) 
	{
		potentialRotation -= p.getRotSpeed();
	}
	if(*buttons[4]) 
	{
		potentialRotation += p.getRotSpeed();
	}
	if(potentialRotation != 0) p.rotate(potentialRotation);
}

void drawGame(sf::RenderWindow& window, Player& p, Arena& a)
{
		window.clear();
		window.draw(p.getSprite());
		window.display();
}

int main()
{
	// keys
	bool escPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;
	bool* buttons[numButtons];
	buttons[0] = &escPressed;
	buttons[1] = &upPressed;
	buttons[2] = &downPressed;
	buttons[3] = &leftPressed;
	buttons[4] = &rightPressed;

	// window definition
	sf::Vector2<uint> wSize(1500, 750);

	// create window
	sf::RenderWindow window(sf::VideoMode(wSize), "Very cool and also fun game");
	window.setFramerateLimit(60);
	window.setPosition({1930, 10});

	// arena definition
	sf::Vector2i aSize(15000, 7500);

	// create arena
	Arena a(aSize);

	// player definition
	sf::Vector2i pSize(41, 41);
	sf::Vector2f pPosition(900, 500);
	sf::Vector2f pSpeed = {0, 0};
	float pRotation = 90;

	float pMaxSpeed = 8;

	float pAcceleration = 0.13;
	float pdrag = 0.008;
	float pRotationSpeed = 4;

	sf::Texture playerTexture;
	if(!playerTexture.loadFromFile("art/basicSpriteL.png"))
		std::cout << "Sprite not loaded :(" << std::endl;

	// create player
	Player p(pSize, pPosition, pSpeed, pRotation, pMaxSpeed, pAcceleration, pdrag, pRotationSpeed, playerTexture);

	// setup timing
	sf::Clock clock;

	while(window.isOpen())
	{
		updateButtonPresses(buttons);
		while(const std::optional event = window.pollEvent())
		{
			if(event->is<sf::Event::Closed>() || escPressed)
			{
				window.close();
			}
		}
		sf::Time elapsed = clock.restart();
		updateGame(elapsed, window, buttons, p, a);
		drawGame(window, p, a);
	}
	window.close();
	return 0;
}
