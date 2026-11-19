#include "Dialogue.hpp"
#include "Console.hpp"

int Dialogue::run(const std::vector<DialogueNode>& nodes, const Character& hero) {
    if (nodes.empty()) {
        return 0;
    }

    int current = 0;
    while (current >= 0) {
        const DialogueNode& node = nodes.at(current);
        std::cout << node.text << '\n';

        std::vector<std::string> options;
        for (const auto& choice : node.choices) {
            options.push_back(choice.label);
        }
        const int selected = Console::choose(options);
        if (selected == 0) {
            return 0;
        }

        const DialogueChoice& choice = node.choices.at(selected - 1);
        const bool passed = choice.stat == Stat::Count ||
                            check(hero.stat(choice.stat), choice.difficulty);
        current = passed ? choice.nextNode : choice.failedNode;
    }
    return -current;
}
