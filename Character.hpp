#pragma once

#include "States.hpp"

#include <array>
#include <cstddef>
#include <vector>

enum class HeroId { WanderingAngel, Seer };
enum class Stat { Strength, Constitution, Dexterity, Intellect, Wisdom, Charisma, Count };
enum class ItemId { None, SerpentFang, HerbPouch, WhistlingDeath };

using Modifiers = std::array<int, static_cast<std::size_t>(Stat::Count)>;

struct Item {
    ItemId id;
    const char* name;
    const char* description;
    bool isWeapon;
    int attackBonus = 0;
    int damageBonus = 0;
};

struct HeroProfile {
    const char* name;
    const char* description;
    const char* background;
    Modifiers modifiers;
    ItemId startingItem;
};

const HeroProfile& profile(HeroId id);
const Item& item(ItemId id);
const char* statName(Stat stat);

class Character {
public:
    explicit Character(HeroId id);

    HeroId id() const;
    int stat(Stat stat) const;
    int hp() const;
    int maxHp() const;
    void takeDamage(int damage);

    States& states();
    const States& states() const;

    const std::vector<ItemId>& inventory() const;
    bool hasItem(ItemId id) const;
    bool addItem(ItemId id);
    bool equip(ItemId id);
    bool discard(ItemId id);
    const Item& weapon() const;
    bool whistleUsed() const;
    bool spendWhistle();
    void dawn();

private:
    friend class SaveFile;

    HeroId id_;
    int hp_;
    States states_;
    std::vector<ItemId> inventory_;
    ItemId equipped_ = ItemId::None;
    bool whistleUsed_ = false;
};

bool check(int value, int difficulty);
