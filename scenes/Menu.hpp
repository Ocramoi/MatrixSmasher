#ifndef MENU_H_
#define MENU_H_

#include "./Scene.cpp"
#include "./Game.hpp"
#include "./Credits.hpp"
#include "./CutScene.hpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/Logic.hpp"
#include "../components/Button.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::shared_ptr;

class Menu : public Scene {
	private:
		shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
						  drawStatic;
		shared_ptr<Scene> curScene;
		shared_ptr<raylib::Window> win;
		// scene_type sceneState;
	public:
		Menu(
			shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
			shared_ptr<Scene>& _curScene
		);
		~Menu();
		void init();
		void draw();
};

#endif