#ifndef CUTSCENE_H_
#define CUTSCENE_H_

#include "./Scene.cpp"
#include "./Credits.hpp"

#include <vector>
#include <memory>
#include <utility>

#include "../components/UIElement.cpp"
#include "../components/SpriteSheet.hpp"
#include "../components/Animation.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::pair;
using std::thread;
using std::move;
using std::shared_ptr;
using std::make_shared;

class CutScene : public Scene {
    private:
        shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
            drawStatic;
        shared_ptr<Scene>* curScene;
        shared_ptr<raylib::Window> win;
        vector<pair<shared_ptr<Animation>, bool>> sprites{};
        unsigned int frameRate{1},
            maxFrames{0};
        shared_ptr<Scene> nextScene{nullptr};
        bool changeState{false};
        thread changeScene;
        void setThread();
    public:
        CutScene(shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene);
        ~CutScene();
        mutex drawMutex;
        void appendSprite(shared_ptr<Animation>& newSprite, bool shouldLoop);
        void appendSprite(SpriteSheet& spriteSheet,  
                        raylib::Vector2 startPos,                    
                        const bool shouldLoop
                    );
        void appendSprite(SpriteSheet& spriteSheet,  
                        raylib::Vector2 startPos,                    
                        float scale,      
                        const bool shouldLoop
                    );
        void setFrameRate(unsigned int _frameRate);
        void init();
        void draw();

        template <class TScene>
        void setScene(TScene _scene) {
            nextScene = _scene;
            setThread();
        }
};

#endif
