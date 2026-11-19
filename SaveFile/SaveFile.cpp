#include "../SaveFile/SaveFile.hpp"

#include <filesystem>
#include <fstream>
#include <utility>

SaveFile::SaveFile(std::string path) : path_(std::move(path)) {}

bool SaveFile::write(const GameState& state) const {
    const std::string temp = path_ + ".tmp";
    std::ofstream out(temp);
    if (!out) {
        return false;
    }

    const Character& hero = state.hero;
    out << "VERMIS_SAVE 1\n"
        << static_cast<int>(state.stage) << ' ' << static_cast<int>(hero.id_) << ' '
        << hero.hp_ << ' ' << static_cast<int>(hero.equipped_) << ' '
        << hero.states_.has(Effect::Curse) << ' ' << hero.whistleUsed_ << '\n'
        << hero.inventory_.size();
    for (ItemId id : hero.inventory_) {
        out << ' ' << static_cast<int>(id);
    }
    out << '\n';
    out.close();

    std::error_code error;
    if (out) {
        std::filesystem::rename(temp, path_, error);
        if (!error) {
            return true;
        }
    }
    std::filesystem::remove(temp, error);
    return false;
}

bool SaveFile::read(GameState& state) const {
    std::ifstream in(path_);
    std::string signature;
    int version = 0;
    int stage = 0;
    int heroId = 0;
    int hp = 0;
    int equipped = 0;
    bool cursed = false;
    bool whistleUsed = false;
    int count = 0;

    if (!(in >> signature >> version) || signature != "VERMIS_SAVE" || version != 1 ||
        !(in >> stage >> heroId >> hp >> equipped >> cursed >> whistleUsed >> count) ||
        stage < 0 || stage > static_cast<int>(ChapterStage::BladeTaken) ||
        heroId < 0 || heroId > static_cast<int>(HeroId::Seer) ||
        equipped < 0 || equipped > static_cast<int>(ItemId::WhistlingDeath) ||
        count < 0 || count > 2) {
        return false;
    }

    const auto chapter = static_cast<ChapterStage>(stage);
    const bool bladeTaken = chapter == ChapterStage::BladeTaken;
    const auto id = static_cast<HeroId>(heroId);
    Character hero(id);
    if (hp < 1 || hp > hero.maxHp() || cursed != bladeTaken ||
        (whistleUsed && !bladeTaken)) {
        return false;
    }

    hero.inventory_.clear();
    hero.equipped_ = ItemId::None;
    for (int i = 0; i < count; ++i) {
        int value = 0;
        if (!(in >> value)) {
            return false;
        }
        const bool isStartingItem = value == static_cast<int>(profile(id).startingItem);
        const bool isLoot = bladeTaken && value == static_cast<int>(ItemId::WhistlingDeath);
        if ((!isStartingItem && !isLoot) || !hero.addItem(static_cast<ItemId>(value))) {
            return false;
        }
    }
    if (equipped != 0 && !hero.equip(static_cast<ItemId>(equipped))) {
        return false;
    }
    in >> std::ws;
    if (!in.eof() || in.bad()) {
        return false;
    }

    if (cursed) {
        hero.states_.add(Effect::Curse);
    }
    hero.hp_ = hp;
    hero.whistleUsed_ = whistleUsed;
    state = {std::move(hero), chapter};
    return true;
}
