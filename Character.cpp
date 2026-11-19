#include "Character.hpp"

#include <algorithm>
#include <utility>

Item::Item(std::string name, ItemType type, int healing, bool cursed)
    : name_(std::move(name)), type_(type), healing_(healing), cursed_(cursed) {}

const std::string& Item::name() const { return name_; }
ItemType Item::type() const { return type_; }
int Item::healing() const { return healing_; }
bool Item::cursed() const { return cursed_; }

int Character::hp() const { return hp_; }
int Character::madness() const { return madness_; }

int Character::stat(Stat stat) const {
    switch (stat) {
        case Stat::Strength: return strength_;
        case Stat::Intellect: return intellect_;
        case Stat::Charisma: return charisma_;
    }
    return 0;
}

void Character::improve(Stat stat) {
    switch (stat) {
        case Stat::Strength: ++strength_; break;
        case Stat::Intellect: ++intellect_; break;
        case Stat::Charisma: ++charisma_; break;
    }
}

void Character::takeDamage(int damage) {
    hp_ = std::max(0, hp_ - damage);
}

void Character::addMadness(int amount) {
    madness_ = std::clamp(madness_ + amount, 0, MaxMadness);
}

const std::vector<Item>& Character::inventory() const {
    return inventory_;
}

bool Character::hasItem(ItemType type) const {
    return std::any_of(inventory_.begin(), inventory_.end(),
                       [type](const Item& item) { return item.type() == type; });
}

void Character::addItem(const Item& item) {
    if (!hasItem(item.type())) {
        inventory_.push_back(item);
    }
}

bool Character::useItem(std::size_t index) {
    if (index >= inventory_.size()) {
        return false;
    }
    hp_ = std::min(MaxHp, hp_ + inventory_[index].healing());
    return discardItem(index);
}

bool Character::discardItem(std::size_t index) {
    if (index >= inventory_.size()) {
        return false;
    }
    inventory_.erase(inventory_.begin() + index);
    return true;
}

bool check(int stat, int difficulty) {
    return stat >= difficulty;
}
