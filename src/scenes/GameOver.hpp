#ifndef GAMEOVER_H_
#define GAMEOVER_H_

#include "./Scene.cpp"
#include "./Menu.hpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/Button.hpp"
#include "../components/Animation.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::shared_ptr;
using std::string;

class GameOver : public Scene {
    private:
        shared_ptr<raylib::Window> win;
        shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
            drawStatic;
        shared_ptr<Scene>* curScene;
        bool changeState{false};
        unsigned int fontSize{20},
            score{0};
        int textPadding{10};
        void _toggleScene(GameOver* _c);
        vector<pair<string, unsigned int>> texts{
                {"GAME OVER!", 40},
                {"You tried your best, but the matrix always wins...", 30},
                {"Final score: ", 25}
        };
        vector<raylib::Vector2> txtPos;
        raylib::Color txtColor{raylib::Color::Green()};
        shared_ptr<Button> butCredits;
        void setUI();
    public:
        mutex drawMutex;
        GameOver(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene
        );
        GameOver(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene,
            unsigned int _score
        );
        ~GameOver();

        void setFinalScore(unsigned int _score);

        void init();
        void draw();
};

#endif // GAMEOVER_H_
