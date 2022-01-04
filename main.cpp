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

using namespace std;

enum _scene {
    MENU = 0,
    GAME = 1,
    OPTIONS = 2,
    CUTSCENE = 3,
    EXIT = -1
};

using scene = _scene;

void _addStack(vector<shared_ptr<UIElement>>& Q, const shared_ptr<UIElement>& el) {
    Q.push_back(el);
}

template <class... UIs>
void _addStack(vector<shared_ptr<UIElement>>& Q, const shared_ptr<UIElement>& el, UIs... els) {
    Q.push_back(el);
    _addStack(Q, els...);
}

template <class... UIs>
vector<shared_ptr<UIElement>> setPosStack(int padding, Alignment align, const shared_ptr<UIElement>& el, UIs... els) {
    vector<shared_ptr<UIElement>> Q{};
    _addStack(Q, el, els...);
    
    int sumHeight{-padding};
    int sumWidth{-padding};
    for (const auto& e : Q) {
        sumHeight += e->getHeight() + padding;
        sumWidth += e->getWidth() + padding;
    }

    int tempHeight{sumHeight/2};
    int tempWidth{sumWidth/2};
    for (auto& e : Q) {
        if (align == VERTICAL) 
            e->setPosition(e->getPosition().Subtract({ e->getWidth()*1.f/2, tempHeight*1.f }));
        else 
            e->setPosition(e->getPosition().Subtract({tempWidth*1.f, e->getHeight()*1.f/2}));

        tempHeight -= e->getHeight() + padding;
        tempWidth -= e->getWidth() + padding;
    }

    return Q;
}

void startAnimation(raylib::Window& win, vector<shared_ptr<UIElement>>& drawStack, int& sceneState) {
    raylib::Vector2 winSize = win.GetSize();
    raylib::Vector2 midSize = winSize.Divide(2.f);
 
    // raylib::Window w1{100, 100, "Virus"};
    // w1.SetPosition(
    //     min(
    //         max(
    //             0.f,
    //             win.GetPosition().x + (rand()*500 - 250)
    //         ),
    //         GetScreenWidth()*1.f
    //     ),
    //     min(
    //         max(
    //             0.f,
    //             win.GetPosition().y + (rand()*300 - 150)
    //         ),
    //         GetScreenHeight()*1.f
    //     )
    // );
    // thread([])

    SpriteSheet boss{raylib::Image{"./resources/spritesheets/boss.png"}, 4, HORIZONTAL};
    boss.scale(0.7);

    SpriteSheet bubble{raylib::Image{"./resources/spritesheets/bubble.png"}, 30, HORIZONTAL};
    bubble.scale(0.5);

    SpriteSheet background{raylib::Image{"./resources/spritesheets/background.png"}, 1, VERTICAL};
    auto backScale = max(
        winSize.x/background.getFrameWidth(), 
        winSize.y/background.getFrameHeight()
    );
    background.scale(backScale);

    auto bossAnimation = make_shared<Animation>(boss, 12U, midSize),
        bubbleAnimation = make_shared<Animation>(bubble, 12U, midSize),
        backgroundAnimation = make_shared<Animation>(background, 12U, raylib::Vector2{ 0.f, 0.f });
    
    bossAnimation->startLoop(true);
    bubbleAnimation->startLoop(false);
    backgroundAnimation->startLoop(true);

    thread([&sceneState] () {
        this_thread::sleep_for(chrono::seconds(3));
        sceneState = GAME;
    }).detach();

    auto Q = setPosStack(10, HORIZONTAL, bossAnimation, bubbleAnimation);
    drawStack.push_back(backgroundAnimation);
    for (auto& e : Q) drawStack.push_back(e);
}

void _deactivateButtons(vector<shared_ptr<UIElement>>& _stack) {
    for (auto& el : _stack) {
        auto cEl = dynamic_cast<Button*>(el.get());
        if (cEl != nullptr) cEl->setClick([]() {});
    }
}

void drawMenu(
    raylib::Window& win, 
    int& sceneState, 
    vector<shared_ptr<UIElement>>& drawStack,
    vector<shared_ptr<UIElement>>& drawStatic
) {
    constexpr int maxButtonWidth = 100;
    raylib::Vector2 winSize = win.GetSize(),
        midSize = winSize.Divide(2.f);

    auto start = make_shared<Button>("Start", midSize, maxButtonWidth), 
        options = make_shared<Button>("Options", midSize, maxButtonWidth),
        exit = make_shared<Button>("Exit", midSize, maxButtonWidth);

    start->setClick([&] () {
        sceneState = CUTSCENE;
        startAnimation(win, drawStatic, sceneState);
    });
    start->setBorder(raylib::Color::DarkGreen());
    start->setFontColor(raylib::Color::DarkGreen());

    options->setClick([&sceneState] () {
        sceneState = scene::OPTIONS;
    });
    options->setBorder(raylib::Color::DarkGreen());
    options->setFontColor(raylib::Color::DarkGreen());

    exit->setClick([&sceneState] () {
        sceneState = scene::EXIT;
    });
    exit->setBorder(raylib::Color::DarkGreen());
    exit->setFontColor(raylib::Color::DarkGreen());

    auto Q = setPosStack(10, VERTICAL, start, options, exit);
    for (auto& e : Q) drawStack.push_back(e);
}

void handleUI(shared_ptr<UIElement>& el, raylib::Mouse& mouseInput) {
    if (mouseInput.IsButtonDown(MOUSE_LEFT_BUTTON)) {
        auto c = el->checkCollision(mouseInput.GetTouchPosition(0));
        if (c.has_value() && c.value())
            el->interact();
    }
    el->draw();
}

bool gameLoop(
    raylib::Window& win, 
    int& sceneState, 
    vector<shared_ptr<UIElement>>& drawStack,
    vector<shared_ptr<UIElement>>& drawStatic
) {
    win.BeginDrawing();
    win.ClearBackground(raylib::Color::DarkGray());

    int copy{sceneState};
    switch (sceneState) {
        case CUTSCENE:
            break;
        case MENU:
            drawMenu(win, sceneState, drawStack, drawStatic);
            break;
        default:
            win.EndDrawing();
            return false;
    }
    if (copy != sceneState) drawStatic.clear();
    
    raylib::Mouse mouseInput;
    for_each(
        drawStack.begin(),
        drawStack.end(),
        [&mouseInput] (decltype(drawStack.front())& el) {
            handleUI(el, mouseInput);
        }
    );
    for_each(
        drawStatic.begin(),
        drawStatic.end(),
        [&mouseInput] (decltype(drawStack.front())& el) {
            handleUI(el, mouseInput);
        }
    );
    drawStack.clear();
    
    win.EndDrawing();
    return true;
}

int main() {
    constexpr int screenWidth = 800,
        screenHeight = 450;

    vector<shared_ptr<UIElement>> drawStack, drawStatic;

    int sceneState{scene::MENU};

    raylib::Image icon{"./resources/imgs/icon.ico"};

    raylib::Window w{screenWidth, screenHeight, "MatrixSmasher"};
    w.SetIcon(icon);
    w.SetTargetFPS(60);
    
    while (gameLoop(w, sceneState, drawStack, drawStatic));
    
    return 0;
}