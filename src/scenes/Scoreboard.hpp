#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include "./Scene.cpp"
#include "./Game.hpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/Button.hpp"
#include "../components/Animation.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::shared_ptr;
using std::string;

class Scoreboard : public Scene {
    private:
        shared_ptr<raylib::Window> win;
        shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
            drawStatic;
        shared_ptr<Scene>* curScene;
        int textPadding{10};
        vector<pair<string, unsigned int>>texts{
                {"SCOREBOARD", 40},
                {"Top 10 best scores currently:", 20}
        };
        vector<raylib::Vector2> txtPos;
        raylib::Color txtColor{raylib::Color::Green()};
        void loadScores(string path = "../data/info.data");
    public:
        Scoreboard(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene
        );
        ~Scoreboard();

        void init();
        void draw();
};

#endif // SCOREBOARD_H_
