#include "States.hpp"

bool States::has(Effect effect) const {
    return active_.at(static_cast<std::size_t>(effect));
}

void States::add(Effect effect) {
    active_.at(static_cast<std::size_t>(effect)) = true;
}

void States::remove(Effect effect) {
    active_.at(static_cast<std::size_t>(effect)) = false;
}

void States::endAttackWindow() {
    remove(Effect::Frightened);
    remove(Effect::AttackAdvantage);
}

void States::dawn() {
    endAttackWindow();
    remove(Effect::WeakToBanishment);
}

const char* States::name(Effect effect) {
    switch (effect) {
        case Effect::Curse: return "Проклятие";
        case Effect::Frightened: return "Испуг";
        case Effect::AttackAdvantage: return "Преимущество при атаках по испуганной цели";
        case Effect::WeakToBanishment: return "Помеха спасброскам против изгнания нежити";
        case Effect::Poisoned: return "Отравление";
        case Effect::Count: return "";
    }
    return "";
}
