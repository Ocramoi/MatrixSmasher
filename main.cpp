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

// void _deactivateButtons(vector<shared_ptr<UIElement>>& _stack) {
//     for (auto& el : _stack) {
//         auto cEl = dynamic_cast<Button*>(el.get());
//         if (cEl != nullptr) cEl->setClick([]() {});
//     }
// }

// void startAnimation(
//     raylib::Window& win, 
//     shared_ptr<vector<shared_ptr<UIElement>>>& drawStack,
//     int& sceneState
// ) {
//     raylib::Vector2 winSize{win.GetSize()},
//         midSize{winSize.Divide(2.f)};
 
//     // raylib::Window w1{100, 100, "Virus"};
//     // w1.SetPosition(
//     //     min(
//     //         max(
//     //             0.f,
//     //             win.GetPosition().x + (rand()*500 - 250)
//     //         ),
//     //         GetScreenWidth()*1.f
//     //     ),
//     //     min(
//     //         max(
//     //             0.f,
//     //             win.GetPosition().y + (rand()*300 - 150)
//     //         ),
//     //         GetScreenHeight()*1.f
//     //     )
//     // );
//     // thread([])

//     SpriteSheet boss{raylib::Image{"./resources/spritesheets/boss.png"}, 4, HORIZONTAL};
//     boss.scale(0.7);

//     SpriteSheet bubble{raylib::Image{"./resources/spritesheets/bubble.png"}, 30, HORIZONTAL};
//     bubble.scale(0.5);

//     SpriteSheet background{raylib::Image{"./resources/spritesheets/background.png"}, 1, VERTICAL};
//     auto backScale = max(
//         winSize.x/background.getFrameWidth(), 
//         winSize.y/background.getFrameHeight()
//     );
//     background.scale(backScale);

//     auto bossAnimation = make_shared<Animation>(boss, 12U, midSize),
//         bubbleAnimation = make_shared<Animation>(bubble, 12U, midSize),
//         backgroundAnimation = make_shared<Animation>(background, 12U, raylib::Vector2{ 0.f, 0.f });
    
//     bossAnimation->startLoop(true);
//     bubbleAnimation->startLoop(false);
//     backgroundAnimation->startLoop(true);

//     thread([&sceneState] () {
//         this_thread::sleep_for(chrono::seconds(3));
//         sceneState = GAME;
//     }).detach();

//     auto Q = setPosStack(10, HORIZONTAL, bossAnimation, bubbleAnimation);
//     drawStack->push_back(backgroundAnimation);
//     for (auto& e : Q) drawStack->push_back(e);
// }

// void drawMenu(
//     raylib::Window& win, 
//     int& sceneState, 
//     shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
//     shared_ptr<vector<shared_ptr<UIElement>>> drawStatic
// ) {
//     constexpr int maxButtonWidth = 100;
//     raylib::Vector2 winSize = win.GetSize(),
//         midSize = winSize.Divide(2.f);

//     auto start = make_shared<Button>("Start", midSize, maxButtonWidth), 
//         options = make_shared<Button>("Options", midSize, maxButtonWidth),
//         exit = make_shared<Button>("Exit", midSize, maxButtonWidth);

//     start->setClick([&] () {
//         sceneState = scene_type::CUTSCENE;
//         startAnimation(win, drawStatic, sceneState);
//     });
//     start->setBorder(raylib::Color::DarkGreen());
//     start->setFontColor(raylib::Color::DarkGreen());

//     options->setClick([&sceneState] () {
//         sceneState = scene_type::OPTIONS;
//     });
//     options->setBorder(raylib::Color::DarkGreen());
//     options->setFontColor(raylib::Color::DarkGreen());

//     exit->setClick([&sceneState] () {
//         sceneState = scene_type::EXIT;
//     });
//     exit->setBorder(raylib::Color::DarkGreen());
//     exit->setFontColor(raylib::Color::DarkGreen());

//     auto Q = setPosStack(10, VERTICAL, start, options, exit);
//     for (auto e : Q) drawStack->push_back(e);
// }

void handleUI(shared_ptr<UIElement>& el, raylib::Mouse& mouseInput) {
    if (mouseInput.IsButtonDown(MOUSE_LEFT_BUTTON)) {
        auto c = el->checkCollision(mouseInput.GetTouchPosition(0));
        if (c.has_value() && c.value())
            el->interact();
    }
    el->draw();
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
    }
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