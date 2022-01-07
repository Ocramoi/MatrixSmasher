#ifndef CREDITS_H_
#define CREDITS_H_

#include "./Scene.cpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/Button.hpp"
#include "../components/Animation.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::shared_ptr;
using std::string;

class Credits : public Scene {
    private:
        shared_ptr<raylib::Window> win;
        shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
            drawStatic;
        shared_ptr<Scene>* curScene;
        pair<shared_ptr<Animation>, bool> sprite;
        bool changeState{false};
        int textPadding{10};
        vector<pair<string, unsigned int>>texts{
                {"MatrixSmasher", 40},
                {"Marco Toledo & Milena Correa da Silva", 20},
                {"ICMC-USP - 2022", 15}
        };
        vector<raylib::Vector2> txtPos;
        raylib::Color txtColor{raylib::Color::Green()};
    public:
        Credits(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene
        );
        Credits(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene,
            vector<pair<string, unsigned int>> extraText
        );
        ~Credits();

        void setSprite(shared_ptr<Animation>& _sprite);
        void setSprite(shared_ptr<Animation>& _sprite, const bool shouldLoop);
        void init();
        void draw();
};

#endif
