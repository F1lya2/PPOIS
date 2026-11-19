#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Console {
public:
    // Ноль означает, что поток ввода закрыт.
    static int choose(const std::vector<std::string>& options) {
        for (std::size_t i = 0; i < options.size(); ++i) {
            std::cout << i + 1 << ". " << options[i] << '\n';
        }

        std::string line;
        while (std::cout << "> " && std::getline(std::cin, line)) {
            std::istringstream input(line);
            int number = 0;
            char extra = '\0';
            if (input >> number && !(input >> extra) && number > 0 &&
                static_cast<std::size_t>(number) <= options.size()) {
                return number;
            }
            std::cout << "Введите номер пункта от 1 до " << options.size() << ".\n";
        }
        return 0;
    }
};
