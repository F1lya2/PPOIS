#pragma once

#include "../SaveFile/SaveFile.hpp"

class Game {
public:
    void run();

private:
    bool selectHero();
    void playChapter();
    void showScene() const;
    void chooseAtCemetery();
    void openInventory();
    void showHero() const;

    GameState state_;
    SaveFile save_{"vermis.sav"};
};
