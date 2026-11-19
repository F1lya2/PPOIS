#pragma once

#include <cstddef>
#include <string>
#include <vector>

enum class Stat { Strength, Intellect, Charisma };
enum class ItemType { Remedy, Relic };

class Item {
public:
    Item(std::string name, ItemType type, int healing, bool cursed);

    const std::string& name() const;
    ItemType type() const;
    int healing() const;
    bool cursed() const;

private:
    std::string name_;
    ItemType type_;
    int healing_;
    bool cursed_;
};

class Character {
public:
    static constexpr int MaxHp = 32;
    static constexpr int MaxMadness = 100;

    int hp() const;
    int madness() const;
    int stat(Stat stat) const;
    void improve(Stat stat);
    void takeDamage(int damage);
    void addMadness(int amount);

    const std::vector<Item>& inventory() const;
    bool hasItem(ItemType type) const;
    void addItem(const Item& item);
    bool useItem(std::size_t index);
    bool discardItem(std::size_t index);

private:

    friend class SaveFile;

    int hp_ = MaxHp;
    int strength_ = 5;
    int intellect_ = 4;
    int charisma_ = 3;
    int madness_ = 0;
    std::vector<Item> inventory_;
};

bool check(int stat, int difficulty);
