#ifndef CREDITS_H_
#define CREDITS_H_

#include "./Scene.cpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/Button.hpp"
#include "../components/Animation.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"
#include "./Menu.hpp"

using std::shared_ptr;

class Credits : public Scene {
	private:
		shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
						  drawStatic;
		shared_ptr<Scene> curScene;
		shared_ptr<raylib::Window> win;
		pair<shared_ptr<Animation>, bool> sprite;
		bool changeState{false};
		void _toggleScene(Credits* _c);
	public:
		Credits() {}
		Credits(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
			shared_ptr<Scene>& _curScene
		);        		

        void setSprite(shared_ptr<Animation>& _sprite);
        void setSprite(shared_ptr<Animation>& _sprite, const bool shouldLoop);
		void init();
		void draw();
};

#endif
