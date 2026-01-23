#ifndef CIRCLEWEAPON_H
#define CIRCLEWEAPON_H

#include "Weapon.h"

class CircleWeapon : public Weapon
{
    protected:
    public:
        CircleWeapon();
        CircleWeapon(const CircleWeapon& other); // copy constructor
        CircleWeapon(CircleWeapon&& other) noexcept; // move constructor
};

#endif
