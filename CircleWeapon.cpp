#include "CircleWeapon.h"


CircleWeapon::CircleWeapon()
{
}
CircleWeapon::CircleWeapon(const CircleWeapon& other) // copy constructor
	: Weapon(other)
{
}
CircleWeapon::CircleWeapon(CircleWeapon&& other) noexcept // move constructor
	: Weapon()
{
}
