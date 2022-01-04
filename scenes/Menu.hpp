#ifndef MENU_H_
#define MENU_H_

#include "./Scene.cpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/Button.hpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::make_shared;
using std::shared_ptr;

class Menu : public Scene {
	private:
		shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
						  drawStatic;
		shared_ptr<raylib::Window> win;
		vector<shared_ptr<UIElement>> buttons{};
		bool changeState{false};
		scene_type sceneState;
		// void setState()
		void _configButtons(vector<shared_ptr<UIElement>>& buttons, const int maxButtonWidth, raylib::Vector2 startPos);
	public:
		Menu() {}
		Menu(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
			);
		vector<shared_ptr<UIElement>> getButtons();
		void init();
		void draw();
};

#endif