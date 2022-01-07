#include "raylib-cpp/include/raylib-cpp.hpp"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "./components/Logic.hpp"
#include "./components/UIElement.cpp"
#include "./components/Button.hpp"
#include "./components/SpriteSheet.hpp"
#include "./components/Animation.hpp"

#include "./scenes/Scene.cpp"
#include "./scenes/Game.hpp"
#include "./scenes/Menu.hpp"

using namespace std;

// TODO scoreboard
// TODO dvd boss

bool handleUI(shared_ptr<UIElement> el, raylib::Mouse& mouseInput) {
    if (el == nullptr) return true;
    el->draw();
    bool r{false};
    if (mouseInput.IsButtonPressed(MOUSE_LEFT_BUTTON)) {
        auto c = el->checkCollision(mouseInput.GetTouchPosition(0));
        if (c.has_value() && c.value()) {
            el->interact();
            r = true;
        }
    }
    return r;
}

bool gameLoop(
    shared_ptr<raylib::Window>& win,
    shared_ptr<Scene>& currentScene,
    shared_ptr<vShrdUIEl> drawStack,
    shared_ptr<vShrdUIEl> drawStatic
) {
    raylib::Mouse mouseInput;
    win->BeginDrawing();
    win->ClearBackground(raylib::Color::DarkGray());

    bool interact{false};
    auto qntStatic = drawStatic->size();
    for (decltype(qntStatic) i{0}; i < qntStatic; ++i) {
        try {
            interact |= handleUI(drawStatic->at(i), mouseInput);
        } catch (...) {
            interact = true;
        }
        if (interact) break;
    }

    if (currentScene == nullptr) {
        win->EndDrawing();
        return false;
    } else
        currentScene->draw();

    for (auto el : *drawStack)
        interact |= handleUI(el, mouseInput);
    drawStack->clear();

    #ifdef DEBUG
    DrawFPS(400, 10);
    #endif

    win->EndDrawing();
    return true;
}

int main() {
    constexpr int screenWidth = 800,
        screenHeight = 450;

    shared_ptr<vShrdUIEl> drawStack{make_shared<vShrdUIEl>()},
        drawStatic{make_shared<vShrdUIEl>()};

    raylib::Image icon{"../resources/imgs/icon.ico"};

    shared_ptr<raylib::Window> w{make_shared<raylib::Window>(screenWidth, screenHeight, "MatrixSmasher")};
    w->SetIcon(icon);
    w->SetTargetFPS(60);

    shared_ptr<Scene> startScene; startScene = make_shared<Menu>(w, drawStack, drawStatic, startScene);
    startScene->init();
    
    while (gameLoop(w, startScene, drawStack, drawStatic));
    
    return 0;
}
