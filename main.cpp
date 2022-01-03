#include <raylib.h>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include "raylib-cpp/include/raylib-cpp.hpp"

#include "./components/UIElement.cpp"
#include "./components/Button.hpp"

using namespace std;

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

void drawMenu(raylib::Window& win, int& sceneState, vector<shared_ptr<UIElement>>& drawStack) {
	constexpr int maxButtonWidth = 100;
    raylib::Vector2 winSize = win.GetSize();
    raylib::Vector2 midSize = winSize.Divide(2.f);

    shared_ptr<Button> start = make_shared<Button>("Start", midSize, maxButtonWidth);
    start->setClick([&sceneState] () {
        sceneState = scene::GAME;
        cout << sceneState << endl;
    });

    shared_ptr<Button> options = make_shared<Button>("Options", midSize, maxButtonWidth);
    options->setClick([&sceneState] () {
        sceneState = scene::OPTIONS;
        cout << sceneState << endl;
    });

    shared_ptr<Button> exit = make_shared<Button>("Exit", midSize, maxButtonWidth);
    exit->setClick([&sceneState] () {
        sceneState = scene::EXIT;
        cout << sceneState << endl;
    });

    auto Q = setPosStack(10, start, options, exit);
    for (auto& e : Q) {
        e->setBorder(raylib::Color::DarkGreen());
        e->setFontColor(raylib::Color::DarkGreen());
        drawStack.push_back(e);
    }
}

void handleUI (shared_ptr<UIElement>& el, raylib::Mouse& mouseInput) {
    if (mouseInput.IsButtonDown(MOUSE_LEFT_BUTTON)) {
        auto c = el->checkCollision(mouseInput.GetTouchPosition(0));
        if (c.has_value() && c.value())
            el->interact();
    }
    el->draw();
}

void gameLoop(raylib::Window& win, int& sceneState, vector<shared_ptr<UIElement>>& drawStack) {
    win.BeginDrawing();
    win.ClearBackground(raylib::Color::DarkGray());
    
    switch (sceneState) {
        case MENU:
            drawMenu(win, sceneState, drawStack);
            break;
        default:
            win.Close();
            break;
    }

    raylib::Mouse mouseInput;
    for (auto& el : drawStack)
        thread([&mouseInput, &el] () { handleUI(el, mouseInput); }).detach();

    win.EndDrawing();
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

    while (!w.ShouldClose())
        gameLoop(w, sceneState, drawStack);
    
    return 0;
}