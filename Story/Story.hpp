#pragma once

#include "Character.hpp"

struct Scene {
    const char* title;
    const char* description;
};

class Story {
public:
    static const Scene& outskirts();
    static const Scene& grotto();
    static const Scene& cemetery();
    static void touchBlade(Character& hero);
};
