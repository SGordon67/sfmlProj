#ifndef CIRCLEWEAPON_H
#define CIRCLEWEAPON_H

#include "Weapon.h"

class CircleWeapon : public Weapon
{
    protected:
        float m_radius;
    public:
        CircleWeapon();
        CircleWeapon(int damage = 10, float cooldown = 0.25f, float radius = 100.0f);
        CircleWeapon(const CircleWeapon& other); // copy constructor
        CircleWeapon(CircleWeapon&& other) noexcept; // move constructor

        void activate(Player& player, QuadTree& quadTree) override;
};

#endif
