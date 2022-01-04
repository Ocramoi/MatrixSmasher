#ifndef MENU_H_
#define MENU_H_

#include "./Scene.cpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

class Menu : public Scene {
	private:
		shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
						  drawStatic;
		shared_ptr<raylib::Window> win;
		bool changeState{false};
	public:
		Menu() {}
		Menu(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
		) : win(_win), drawStack(_drawStack), drawStatic(_drawStatic) {};
		void init();
		void draw();
};

#endif