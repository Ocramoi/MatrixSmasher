#ifndef GAME_H_
#define GAME_H_

#include "./Scene.cpp"
#include "./GameOver.hpp"

#include <mutex>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <queue>
#include <fstream>
#include <atomic>
#include <condition_variable>

#include "../raylib-cpp/include/raylib-cpp.hpp"
#include "../components/UIElement.cpp"
#include "../components/Game/Word.hpp"

using std::mutex;
using std::thread;
using std::vector;
using std::queue;
using std::lock_guard;
using std::ifstream;
using std::string;
using std::default_random_engine;
using std::atomic_bool;
using std::condition_variable;
using std::unique_lock;

using score_t = uint32_t;
using difficulty_t = uint32_t;

constexpr unsigned int SCOREBOARD_SIZE{10};

class Game : public Scene {
    private:
        static void _feed(Game* _game);
        void loadWords(string path = "../resources/wordlist.txt");
        void endGame();
        void saveScore(string path = "../data/info.data");
        void loadOptions(string path = "../data/options.txt");
        shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
            drawStatic;
        shared_ptr<Scene>* curScene;
        shared_ptr<raylib::Window> win;
        vector<string> availableWords;
        queue<Word> words;
        bool changeState{false};
        float var{0.2f}, speed{3000.f},
                wordSpeed{15.f};
        int lives{10};
        unsigned int fontSize{20};
        mutex wlMutex,
                feedMutex;
        condition_variable conditionalFeed;
        atomic_bool stopFeed{false};
        thread feeder;
        default_random_engine rE;
        score_t points{0};
        difficulty_t diff{0};
    public:
        mutex drawMutex;
        Game(
            shared_ptr<raylib::Window>& _win,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
            shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
            shared_ptr<Scene>& _curScene
        );
        ~Game();
        void init();
        void draw();
        void setSpeed(unsigned int _speed);
        void getSpeed();
};

#endif
