#pragma once

#include "../Character/Character.hpp"

#include <string>
#include <vector>

struct DialogueChoice {
    std::string label;
    int nextNode = -1;
    Stat stat = Stat::Count;
    int difficulty = 0;
    int failedNode = -1;
};

struct DialogueNode {
    std::string text;
    std::vector<DialogueChoice> choices;
};

class Dialogue {
public:
    // Отрицательный узел завершает разговор. Результат 0 означает конец ввода.
    static int run(const std::vector<DialogueNode>& nodes, const Character& hero);
};
