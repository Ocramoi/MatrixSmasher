#include "raylib-cpp/include/raylib-cpp.hpp"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "./components/UIElement.cpp"
#include "./components/Button.hpp"
#include "./components/SpriteSheet.hpp"
#include "./components/Animation.hpp"

#include "./scenes/Scene.cpp"
#include "./scenes/Game.hpp"
#include "./scenes/Menu.hpp"

using namespace std;

bool handleUI(shared_ptr<UIElement>& el, raylib::Mouse& mouseInput) {
    bool r{false};
    if (mouseInput.IsButtonDown(MOUSE_LEFT_BUTTON)) {
        auto c = el->checkCollision(mouseInput.GetTouchPosition(0));
        if (c.has_value() && c.value()) {
            el->interact();
            r = true;
        }
    }
    // TODO acesso atomico + curscene compartilhado
    el->draw();
    return r;
}

bool gameLoop(
    shared_ptr<raylib::Window>& win,
    shared_ptr<Scene>& currentScene,
    shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>> drawStatic
) {
    win->BeginDrawing();
    win->ClearBackground(raylib::Color::DarkGray());

    if (currentScene == nullptr) {
        win->EndDrawing();
        return false;
    } else
        currentScene->draw();
    
    raylib::Mouse mouseInput;
    for (auto& el : *drawStack)
        handleUI(el, mouseInput);
    for (auto& el : *drawStatic)
        handleUI(el, mouseInput);
    drawStack->clear();

    win->EndDrawing();
    return true;
}

int main() {
    constexpr int screenWidth = 800,
        screenHeight = 450;

    shared_ptr<vector<shared_ptr<UIElement>>> drawStack{make_shared<vector<shared_ptr<UIElement>>>()},
        drawStatic{make_shared<vector<shared_ptr<UIElement>>>()};

    raylib::Image icon{"./resources/imgs/icon.ico"};

    shared_ptr<raylib::Window> w{make_shared<raylib::Window>(screenWidth, screenHeight, "MatrixSmasher")};
    w->SetIcon(icon);
    w->SetTargetFPS(60);

    shared_ptr<Scene> startScene; startScene = make_shared<Menu>(w, drawStack, drawStatic, startScene);
    startScene->init();
    
    while (gameLoop(w, startScene, drawStack, drawStatic));
    
    return 0;
}
