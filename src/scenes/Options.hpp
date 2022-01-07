#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "./Scene.cpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/Slider.hpp"
#include "../components/Button.hpp"
#include "../components/Logic.hpp"
#include "../components/Animation.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::shared_ptr;
using std::thread;

class Options : public Scene {
    private:
        shared_ptr<raylib::Window> win;
        shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
            drawStatic;
        shared_ptr<Scene>* curScene;
        pair<shared_ptr<Animation>, bool> sprite;
        bool changeState{false};
        void _toggleScene(Options* _c);
        thread keyboardCapture;
        shared_ptr<Slider> speed;
    public:
        mutex drawMutex;
        Options(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene
        );
        ~Options();

        void setSprite(shared_ptr<Animation>& _sprite);
        void setSprite(shared_ptr<Animation>& _sprite, const bool shouldLoop);
        void init();
        void draw();
};

#endif
