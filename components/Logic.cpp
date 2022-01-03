#include "Logic.hpp"

#include <unordered_map>
#include <optional>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

using std::to_string;
using std::vector;
using std::stringstream;
using std::max;

constexpr auto HEADER_FONT_SIZE{20};

// void Logic::checkCollisions(
//     const raylib::Vector2 &point,
//     Game &game
// ) {
//     game.inCollision = nullptr;
//     for (const auto &actor : game.scene.actors) {
//         auto collCheck = actor->checkCollision(point);
//         if (collCheck.has_value() && collCheck.value()) {
//             game.inCollision = actor;
//             break;
//         }
//     }
// }

// std::optional<textureMap> Logic::loadTextures(std::string path) {
//     vector<string> files = raylib::GetDirectoryFiles(path);
//     if (files.size() <= Logic::RESOURCE_OFFSET) return {};

//     textureMap um;
//     for (auto file : files)
//         if (file[0] != '.') um[file] = std::make_shared<raylib::Texture>(path + "/" + move(file));

//     return um;
// }

// void Logic::drawHeader(Game &game, const raylib::Window &win) {
//     string msg = "Scene: " + string(game.scene.getStr());
//     raylib::DrawText(msg, 10, 10, 20, raylib::Color::LightGray());
// }

// void Logic::drawFPS(Game &game, const raylib::Window &win) {
//     auto strFPS{to_string(win.GetFPS())};
//     auto strSize{raylib::MeasureText(strFPS, HEADER_FONT_SIZE)};
//     raylib::DrawText(
//         strFPS,
//         win.GetWidth() - strSize - 10,
//         10,
//         HEADER_FONT_SIZE,
//         raylib::Color::DarkBlue()
//     );
// }

pair<string, unsigned short>Logic::clampText(
    const string &str,
    int maxWidth,
    int fontSize) {
    unsigned int ignore{0};
    return clampText(str, maxWidth, fontSize, ignore);
}

pair<string, unsigned short>Logic::clampText(
    const string &str,
    int maxWidth,
    int fontSize,
    unsigned int& maxTok) {
    stringstream scriptStream{str};
    string temp,
        line{""},
        clamped{""};
    unsigned short nLines{1};
    maxTok = 0;
    while (std::getline(scriptStream, temp, ' ')) {
        auto curLen = raylib::MeasureText(line + temp, fontSize);
        maxTok = max<int>(maxTok, curLen);
        if (curLen > maxWidth) {
            clamped.append(line + "\n");
            line.clear();
            nLines++;
        }
        line.append(temp + " ");
    }
    if (!line.empty()) clamped.append(line);
    else nLines--;
    return { clamped, nLines };
}
