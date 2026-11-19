#include "Game.hpp"
#include "Console.hpp"
#include "Dialogue.hpp"
#include "Story.hpp"

#include <array>
#include <iomanip>
#include <string>
#include <vector>

namespace {
void printScene(const Scene& scene) {
    std::cout << "\n== " << scene.title << " ==\n" << scene.description << '\n';
}

void printItem(const Item& entry) {
    std::cout << '\n' << entry.name << '\n' << entry.description << '\n';
}

void printModifiers(const Character& hero) {
    for (int i = 0; i < static_cast<int>(Stat::Count); ++i) {
        const auto stat = static_cast<Stat>(i);
        const int modifier = hero.stat(stat);
        std::cout << statName(stat) << ' ';
        if (modifier > 0) {
            std::cout << std::showpos << modifier << std::noshowpos;
        } else {
            std::cout << modifier;
        }
        std::cout << '\n';
    }
}

void printWeapon(const Character& hero) {
    const Item& weapon = hero.weapon();
    std::cout << "Оружие: " << weapon.name << '\n';
    if (weapon.attackBonus != 0 || weapon.damageBonus != 0) {
        std::cout << std::showpos << "Бонус к атаке " << weapon.attackBonus
                  << " | Бонус к урону " << weapon.damageBonus << std::noshowpos << '\n';
    }
}
}

void Game::run() {
    while (std::cin) {
        std::cout << "\n=== VERMIS ===\n";
        switch (Console::choose({"Новая игра", "Загрузить игру", "Выход"})) {
            case 1:
                if (selectHero()) {
                    playChapter();
                }
                break;
            case 2:
                if (save_.read(state_)) {
                    std::cout << "Сохранение загружено.\n";
                    playChapter();
                } else {
                    std::cout << "Сохранение отсутствует или повреждено.\n";
                }
                break;
            default:
                return;
        }
    }
}

bool Game::selectHero() {
    const std::array<HeroId, 2> heroes{HeroId::WanderingAngel, HeroId::Seer};
    while (std::cin) {
        const int choice = Console::choose({
            profile(heroes[0]).name, profile(heroes[1]).name, "Назад"
        });
        if (choice == 0 || choice == 3) {
            return false;
        }

        const HeroId id = heroes[choice - 1];
        const HeroProfile& entry = profile(id);
        const Character hero(id);
        std::cout << '\n' << entry.name << '\n' << entry.description
                  << "\n\nПредыстория:\n" << entry.background << "\n\n";
        printModifiers(hero);
        printItem(item(entry.startingItem));

        if (Console::choose({"Начать путешествие", "Вернуться к выбору"}) == 1) {
            state_ = {hero, ChapterStage::Outskirts};
            return true;
        }
    }
    return false;
}

void Game::playChapter() {
    showScene();
    while (std::cin) {
        std::cout << '\n';
        const ChapterStage stage = state_.stage;
        const bool finished = stage == ChapterStage::BladeLeft ||
                              stage == ChapterStage::BladeTaken;
        const int menu = Console::choose({
            stage == ChapterStage::Outskirts ? "Отправиться на кладбище" :
            finished ? "Посмотреть итог главы" : "Подойти к гробу",
            "Персонаж и состояния", "Инвентарь", "Сохранить игру", "Главное меню"
        });

        switch (menu) {
            case 1:
                if (stage == ChapterStage::Outskirts) {
                    state_.stage = ChapterStage::Cemetery;
                    showScene();
                } else if (finished) {
                    showScene();
                } else {
                    chooseAtCemetery();
                }
                break;
            case 2:
                showHero();
                break;
            case 3:
                openInventory();
                break;
            case 4:
                std::cout << (save_.write(state_) ? "Игра сохранена.\n"
                                                   : "Ошибка записи сохранения.\n");
                break;
            default:
                return;
        }
    }
}

void Game::showScene() const {
    switch (state_.stage) {
        case ChapterStage::Outskirts:
            printScene(Story::outskirts());
            printScene(Story::grotto());
            break;
        case ChapterStage::Cemetery:
            printScene(Story::cemetery());
            break;
        case ChapterStage::BladeLeft:
            std::cout << "Свистящая смерть осталась в гробу.\nКонец главы.\n";
            break;
        case ChapterStage::BladeTaken:
            std::cout << "Мёртвый воин побеждён. Свистящая смерть была взята.\nКонец главы.\n";
            break;
    }
}

void Game::chooseAtCemetery() {
    Character& hero = state_.hero;
    const int choice = Dialogue::run({
        {"Взять Свистящую смерть?", {
            {"Взять оружие", -1},
            {"Оставить оружие", -2},
        }}
    }, hero);

    if (choice == 1) {
        Story::touchBlade(hero);
        std::cout << "Состояние получено: " << States::name(Effect::Curse) << ".\n"
                  << "Мёртвый воин пробуждается. Начинается бой.\nПобеда.\n";
        // По путеводителю пошаговый бой в этой сцене пока заменён победой по сюжету.
        hero.addItem(ItemId::WhistlingDeath);
        hero.equip(ItemId::WhistlingDeath);
        state_.stage = ChapterStage::BladeTaken;
        printItem(hero.weapon());
        printWeapon(hero);
        showScene();
    } else if (choice == 2) {
        state_.stage = ChapterStage::BladeLeft;
        showScene();
    }
}

void Game::openInventory() {
    Character& hero = state_.hero;
    std::vector<std::string> options;
    for (ItemId id : hero.inventory()) {
        const Item& entry = item(id);
        options.emplace_back(entry.name);
        if (hero.weapon().id == id) {
            options.back() += " (в руках)";
        }
    }
    if (options.empty()) {
        std::cout << "Инвентарь пуст.\n";
        return;
    }
    options.push_back("Назад");

    const int choice = Console::choose(options);
    if (choice == 0 || static_cast<std::size_t>(choice) > hero.inventory().size()) {
        return;
    }

    const ItemId id = hero.inventory()[choice - 1];
    const Item& entry = item(id);
    printItem(entry);
    if (entry.isWeapon && hero.weapon().id != id) {
        switch (Console::choose({"Взять в руки", "Выбросить", "Назад"})) {
            case 1: hero.equip(id); break;
            case 2: hero.discard(id); break;
            default: break;
        }
    } else if (Console::choose({"Выбросить", "Назад"}) == 1) {
        hero.discard(id);
    }
}

void Game::showHero() const {
    const Character& hero = state_.hero;
    std::cout << '\n' << profile(hero.id()).name << '\n';
    printModifiers(hero);
    printWeapon(hero);
    std::cout << "Состояния:";
    bool any = false;
    for (int i = 0; i < static_cast<int>(Effect::Count); ++i) {
        const auto effect = static_cast<Effect>(i);
        if (hero.states().has(effect)) {
            std::cout << (any ? ", " : " ") << States::name(effect);
            any = true;
        }
    }
    std::cout << (any ? ".\n" : " нет.\n");
}
