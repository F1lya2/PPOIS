#include "../Combat/Combat.hpp"

#include <algorithm>

Combat::Combat(Character& hero, Opponent& target) : hero_(hero), target_(target) {}

bool Combat::frighteningWhistle(int distanceInFeet) {
    if (hero_.hp() <= 0 || target_.hp <= 0 || distanceInFeet < 0 ||
        distanceInFeet > 5 || !hero_.spendWhistle()) {
        return false;
    }
    target_.states.add(Effect::Frightened);
    hero_.states().add(Effect::AttackAdvantage);
    return true;
}

int Combat::attack(int firstRoll, int secondRoll, int weaponDamage) {
    if (hero_.hp() <= 0 || target_.hp <= 0 || weaponDamage < 0) {
        return 0;
    }

    const Item& weapon = hero_.weapon();
    const bool advantage = hero_.states().has(Effect::AttackAdvantage) &&
                           target_.states.has(Effect::Frightened);
    const int roll = advantage ? std::max(firstRoll, secondRoll) : firstRoll;
    if (!check(roll + hero_.stat(Stat::Strength) + weapon.attackBonus, target_.defense)) {
        return 0;
    }

    int damage = weaponDamage + weapon.damageBonus;
    if (weapon.id == ItemId::WhistlingDeath && target_.undead) {
        damage += weaponDamage / 2;
        target_.states.add(Effect::WeakToBanishment);
    }
    if (weapon.id == ItemId::SerpentFang) {
        target_.states.add(Effect::Poisoned);
    }
    damage = std::min(damage, target_.hp);
    target_.hp -= damage;
    return damage;
}

void Combat::defend() { defending_ = true; }

int Combat::receiveAttack(int damage) {
    if (target_.hp <= 0) {
        return 0;
    }
    const int previousHp = hero_.hp();
    hero_.takeDamage(defending_ ? std::max(0, damage) / 2 : damage);
    return previousHp - hero_.hp();
}

bool Combat::targetResistsBanishment(int firstRoll, int secondRoll, int difficulty) const {
    const int roll = target_.states.has(Effect::WeakToBanishment)
                         ? std::min(firstRoll, secondRoll) : firstRoll;
    return check(roll, difficulty);
}

void Combat::beginHeroTurn() {
    defending_ = false;
    hero_.states().endAttackWindow();
    target_.states.endAttackWindow();
}

void Combat::dawn() {
    hero_.dawn();
    target_.states.dawn();
}
