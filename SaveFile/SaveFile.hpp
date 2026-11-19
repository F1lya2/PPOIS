#pragma once

#include "../Character/Character.hpp"

#include <string>

enum class ChapterStage { Outskirts, Cemetery, BladeLeft, BladeTaken };

struct GameState {
    Character hero{HeroId::WanderingAngel};
    ChapterStage stage = ChapterStage::Outskirts;
};

class SaveFile {
public:
    explicit SaveFile(std::string path);

    bool write(const GameState& state) const;
    bool read(GameState& state) const;

private:
    std::string path_;
};
