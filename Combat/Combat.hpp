#pragma once

#include "../Character/Character.hpp"

struct Opponent {
    int hp;
    int defense;
    bool undead;
    States states{};
};

class Combat {
public:
    Combat(Character& hero, Opponent& target);

    bool frighteningWhistle(int distanceInFeet);
    int attack(int firstRoll, int secondRoll, int weaponDamage);
    void defend();
    int receiveAttack(int damage);
    bool targetResistsBanishment(int firstRoll, int secondRoll, int difficulty) const;
    void beginHeroTurn();
    void dawn();

private:
    Character& hero_;
    Opponent& target_;
    bool defending_ = false;
};
