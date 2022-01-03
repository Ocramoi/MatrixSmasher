#include "raylib-cpp/include/raylib-cpp.hpp"
#include <raylib.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./components/UIElement.cpp"
#include "./components/Button.hpp"
#include "./components/SpriteSheet.hpp"
#include "./components/Animation.hpp"

using namespace std;

shared_ptr<SpriteSheet> boss;
shared_ptr<Animation> bossAnimation;

enum _scene {
    MENU = 0,
    GAME = 1,
    OPTIONS = 2,
    EXIT = -1
};

using scene = _scene;

void _addStack(vector<shared_ptr<Button>>& Q, const shared_ptr<Button>& el) {
    Q.push_back(el);
}

template <class UIE, class... UIEls>
void _addStack(vector<UIE>& Q, const UIE& el, UIEls... els) {
    Q.push_back(el);
    _addStack(Q, els...);
}

template <class UI, class... UIs>
vector<shared_ptr<Button>> setPosStack(int padding, UI el, UIs... els) {
    vector<shared_ptr<Button>> Q{};
    _addStack(Q, el, els...);
    
    int sumHeight{0};
    for (const auto& e : Q) sumHeight += e->getHeight() + padding;
    sumHeight -= padding;

    int tempHeight{sumHeight/2};
    for (auto& e : Q) {
        e->setPosition(e->getPosition().Subtract({ e->getWidth()*1.f/2, tempHeight*1.f }));
        tempHeight -= e->getHeight() + padding;
    }

    return Q;
}

void startAnimation(raylib::Window& win) {
    SpriteSheet boss{raylib::Image{"./resources/imgs/icon.ico"}, 4, HORIZONTAL};
    SpriteSheet bubble{raylib::Image{"./resources/imgs/icon.ico"}, 30, VERTICAL};

    Animation bossAnimation{boss, 6, {0, 0}};
    Animation bubbleAnimation{bubble, 6, {100, 100}};
    bossAnimation.startLoop(true);
}

void drawMenu(raylib::Window& win, int& sceneState, vector<shared_ptr<UIElement>>& drawStack) {
	constexpr int maxButtonWidth = 100;
    raylib::Vector2 winSize = win.GetSize();
    raylib::Vector2 midSize = winSize.Divide(2.f);

    shared_ptr<Button> start = make_shared<Button>("Start", midSize, maxButtonWidth);
    start->setClick([&sceneState] () {
        sceneState = scene::GAME;
    });

    shared_ptr<Button> options = make_shared<Button>("Options", midSize, maxButtonWidth);
    options->setClick([&sceneState] () {
        sceneState = scene::OPTIONS;
    });

    shared_ptr<Button> exit = make_shared<Button>("Exit", midSize, maxButtonWidth);
    exit->setClick([&sceneState] () {
        sceneState = scene::EXIT;
    });

    auto Q = setPosStack(10, start, options, exit);
    for (auto& e : Q) {
        e->setBorder(raylib::Color::DarkGreen());
        e->setFontColor(raylib::Color::DarkGreen());
        drawStack.push_back(e);
    }
    // SpriteSheet boss{raylib::Image{"./resources/spritesheets/menu.png"}, 4, HORIZONTAL};

    // shared_ptr<Animation> bossAnimation = make_shared<Animation>(boss, static_cast<unsigned int>(6), raylib::Vector2{0, 0});
    // bossAnimation->startLoop(true);
    // drawStack.push_back(bossAnimation);
}

void handleUI(shared_ptr<UIElement>& el, raylib::Mouse& mouseInput) {
    if (mouseInput.IsButtonDown(MOUSE_LEFT_BUTTON)) {
        auto c = el->checkCollision(mouseInput.GetTouchPosition(0));
        if (c.has_value() && c.value())
            el->interact();
    }
    el->draw();
}

bool gameLoop(raylib::Window& win, int& sceneState, vector<shared_ptr<UIElement>>& drawStack) {
    win.BeginDrawing();
    win.ClearBackground(raylib::Color::DarkGray());
    
    switch (sceneState) {
        case MENU:
            drawMenu(win, sceneState, drawStack);
            break;
        default:
            win.EndDrawing();
            return false;
    }
    
    raylib::Mouse mouseInput;
    for_each(
        drawStack.begin(),
        drawStack.end(),
        [&mouseInput] (decltype(drawStack.front())& el) {
            handleUI(el, mouseInput);
        }
    );

    bossAnimation->draw();

    drawStack.clear();
    win.EndDrawing();
    
    return true;
}

int main() {
	constexpr int screenWidth = 800,
    screenHeight = 450;

    vector<shared_ptr<UIElement>> drawStack;

    int sceneState{scene::MENU};

    raylib::Image icon{"./resources/imgs/icon.ico"};

    raylib::Window w{screenWidth, screenHeight, "MatrixSmasher"};
    w.SetIcon(icon);
    w.SetTargetFPS(60);

    boss = make_shared<SpriteSheet>(raylib::Image{"./resources/spritesheets/menu.png"}, 4, HORIZONTAL);
    bossAnimation = make_shared<Animation>(*boss, static_cast<unsigned int>(6), raylib::Vector2{0, 0});
    bossAnimation->startLoop(true);

    while (gameLoop(w, sceneState, drawStack));
    
    return 0;
}