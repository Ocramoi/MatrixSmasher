#include <raylib.h>
#include "raylib-cpp/include/raylib-cpp.hpp"
#include <iostream>
#include <thread>
#include <vector>

#include "components/Button.hpp"

using namespace std;

void _addStack(vector<Button>& Q, const Button& el) {
    Q.push_back(el);
}

template <class UIE, class... UIEls>
void _addStack(vector<UIE>& Q, UIE el, UIEls... els) {
    Q.push_back(el);
    _addStack(Q, els...);
}

// template <class UIEl, class... UIEls>
// void setPosStack(UIEl el,)

template <class UI, class... UIs>
vector<Button> setPosStack(int padding, UI el, UIs... els) {
    vector<Button> Q{};
    _addStack(Q, el, els...);
    
    int sumHeight{0};
    for (auto e : Q) sumHeight += e.getHeight() + padding;
    sumHeight -= padding;

    int tempHeight{sumHeight/2};
    for (auto& e : Q) {
        e.setPosition(e.getPosition().Subtract({ e.getWidth()*1.f/2, tempHeight*1.f }));
        tempHeight -= e.getHeight() + padding;
    }
    
    return Q;
}

void drawMenu(raylib::Window& win) {
	constexpr int maxButtonWidth = 100;
    raylib::Vector2 winSize = win.GetSize();
    raylib::Vector2 midSize = winSize.Divide(2.0);

    Button start{"Start", midSize, maxButtonWidth};
    // start.setClick();

    Button options{"Options", midSize, maxButtonWidth};
    // options.setClick();

    Button exit{"Exit", midSize, maxButtonWidth};
    // exit.setClick();

    auto Q = setPosStack(10, start, options, exit);
    for (auto& e : Q) {
        e.setBorder(raylib::Color::DarkGreen());
        e.setFontColor(raylib::Color::DarkGreen());
        e.draw();
    }
}

void gameLoop(raylib::Window& win) {
    win.BeginDrawing();
    win.ClearBackground(raylib::Color::DarkGray());
    
    drawMenu(win);

    win.EndDrawing();
}

int main() {
	constexpr int screenWidth = 800,
    screenHeight = 450;

    raylib::Image icon{"./resources/imgs/icon.ico"};

    raylib::Window w{screenWidth, screenHeight, "MatrixSmasher"};
    w.SetIcon(icon);
    w.SetTargetFPS(60);

    while (!w.ShouldClose()) {
        gameLoop(w);
    }
    
    return 0;
}