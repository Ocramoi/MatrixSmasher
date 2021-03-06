#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <memory>
#include <mutex>

#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::vector;
using std::make_shared;
using std::shared_ptr;
using std::mutex;

enum _scene {
    MENU = 0,
    GAME = 1,
    OPTIONS = 2,
    CUTSCENE = 3,
    EXIT = -1
};
using scene_type = _scene;

class Scene {
    public:
        mutex drawMutex;
        virtual ~Scene() = default;
        virtual void init() = 0;
        virtual void draw() = 0;
};

#endif
