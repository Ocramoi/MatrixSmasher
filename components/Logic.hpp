#ifndef LOGIC_H_
#define LOGIC_H_

#include <optional>
#include <iostream>
#include <string_view>
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::string_view;
using std::string;
using std::optional;
using std::pair;

namespace Logic {
    constexpr string_view PROJECT = "Storytelling",
        RESOURCES_PATH = "resources/";
    constexpr int RESOURCE_OFFSET = 2;

    // void drawHeader(Game &game, const raylib::Window &win);
    // void drawFPS(Game &game, const raylib::Window &win);
    pair<string, unsigned short> clampText(
        const string &str,
        int maxWidth,
        int fontSize);
    pair<string, unsigned short> clampText(
        const string &str,
        int maxWidth,
        int fontSize,
        unsigned int& maxTok);
}

#endif // LOGIC_H_
