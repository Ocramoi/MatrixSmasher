#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <memory>

#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::vector;
using std::make_shared;
using std::shared_ptr;

class Scene {
	public:
		virtual ~Scene() = default;
		virtual void init() = 0;
		virtual void draw() = 0;
};

#endif