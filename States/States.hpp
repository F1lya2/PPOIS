#pragma once

#include <array>
#include <cstddef>

enum class Effect {
    Curse, Frightened, AttackAdvantage, WeakToBanishment, Poisoned, Count
};

class States {
public:
    bool has(Effect effect) const;
    void add(Effect effect);
    void remove(Effect effect);
    void endAttackWindow();
    void dawn();
    static const char* name(Effect effect);

private:
    std::array<bool, static_cast<std::size_t>(Effect::Count)> active_{};
};
