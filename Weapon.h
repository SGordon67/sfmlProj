#ifndef WEAPON_H
#define WEAPON_H

class Player;

class Weapon
{
    protected:
        int m_damage;
        float m_cooldown;
        float m_timeSinceLastFire;
        bool m_active;

    public:
        virtual ~Weapon() = default;

        virtual void update(float deltaTime, Player& player);

};

#endif
