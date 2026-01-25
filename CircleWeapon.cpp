#include "CircleWeapon.h"
#include <algorithm>


CircleWeapon::CircleWeapon()
    : Weapon()
      , m_radius(50)
{
}
CircleWeapon::CircleWeapon(const CircleWeapon& other) // copy constructor
	: Weapon(other)
      , m_radius(other.m_radius)
{
}
CircleWeapon::CircleWeapon(CircleWeapon&& other) noexcept // move constructor
    : Weapon(std::move(other))
      , m_radius(other.m_radius)
{
}
