#include "CircleWeapon.h"
#include <algorithm>


CircleWeapon::CircleWeapon()
{
}
CircleWeapon::CircleWeapon(const CircleWeapon& other) // copy constructor
	: Weapon(other)
{
}
CircleWeapon::CircleWeapon(CircleWeapon&& other) noexcept // move constructor
	: Weapon(std::move(other))
{
}
