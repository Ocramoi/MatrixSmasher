#ifndef MENU_H_
#define MENU_H_

#include "./Scene.cpp"
#include "./Game.hpp"
#include "./Credits.hpp"
#include "./Options.hpp"
#include "./CutScene.hpp"

#include <vector>
#include <memory>

#include "../components/Logic.hpp"
#include "../components/UIElement.cpp"
#include "../components/Button.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::shared_ptr;
using std::weak_ptr;

class Menu : public Scene {
    private:
        shared_ptr<raylib::Window> win;
        shared_ptr<vShrdUIEl> drawStack,
            drawStatic;
        shared_ptr<Scene>* curScene;
        void clearElements();
        string title{"MatrixSmasher"};
        unsigned titleSize{40};
        raylib::Vector2 titlePos{0.f, 0.f};
    public:
        mutex drawMutex;
        Menu(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vShrdUIEl>& _drawStack,
            shared_ptr<vShrdUIEl>& _drawStatic,
            shared_ptr<Scene>& _curScene
        );
        ~Menu();
        void init();
        void draw();
};

#endif
