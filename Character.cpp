#include "Character.hpp"

#include <algorithm>

const char* statName(Stat stat) {
    static constexpr std::array<const char*, 6> Names{
        "Сила", "Телосложение", "Ловкость", "Интеллект", "Мудрость", "Харизма"
    };
    return Names.at(static_cast<std::size_t>(stat));
}

Character::Character(HeroId id) : id_(id), hp_(maxHp()) {
    const ItemId startingItem = profile(id_).startingItem;
    addItem(startingItem);
    equip(startingItem);
}

HeroId Character::id() const { return id_; }

int Character::stat(Stat stat) const {
    return profile(id_).modifiers.at(static_cast<std::size_t>(stat));
}

int Character::hp() const { return hp_; }
int Character::maxHp() const { return 10 + stat(Stat::Constitution); }

void Character::takeDamage(int damage) {
    hp_ -= std::clamp(damage, 0, hp_);
}

States& Character::states() { return states_; }
const States& Character::states() const { return states_; }

const std::vector<ItemId>& Character::inventory() const { return inventory_; }

bool Character::hasItem(ItemId id) const {
    return std::find(inventory_.begin(), inventory_.end(), id) != inventory_.end();
}

bool Character::addItem(ItemId id) {
    if (id == ItemId::None || hasItem(id)) {
        return false;
    }
    inventory_.push_back(id);
    return true;
}

bool Character::equip(ItemId id) {
    if (!hasItem(id) || !item(id).isWeapon) {
        return false;
    }
    equipped_ = id;
    return true;
}

bool Character::discard(ItemId id) {
    const auto position = std::find(inventory_.begin(), inventory_.end(), id);
    if (position == inventory_.end()) {
        return false;
    }
    inventory_.erase(position);
    if (equipped_ == id) {
        equipped_ = ItemId::None;
    }
    return true;
}

const Item& Character::weapon() const { return item(equipped_); }
bool Character::whistleUsed() const { return whistleUsed_; }

bool Character::spendWhistle() {
    if (whistleUsed_ || equipped_ != ItemId::WhistlingDeath) {
        return false;
    }
    whistleUsed_ = true;
    return true;
}

void Character::dawn() {
    whistleUsed_ = false;
    states_.dawn();
}

bool check(int value, int difficulty) { return value >= difficulty; }
