#ifndef GAME_H_
#define GAME_H_

#include "./Scene.cpp"

#include <mutex>
#include <thread>
#include <chrono>
#include <random>
#include <vector>
#include <queue>

#include "../raylib-cpp/include/raylib-cpp.hpp"
#include "../components/UIElement.cpp"
#include "../components/Game/Word.hpp"

using std::mutex;
using std::thread;
using std::vector;
using std::queue;
using std::lock_guard;

class Game : public Scene {
	private:
		shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
			drawStatic;
		shared_ptr<raylib::Window> win;
		
		queue<Word> words;
		bool changeState{false};
		float var{0.2f}, speed{3000.f};
		static void _feed(Game* _game);
		int lives{5};
		mutex wlMutex;
		thread feeder;
		void endGame();
	public:
		Game(
			shared_ptr<raylib::Window>& _win, 
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack, 
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
		);
		void init();
		void draw();
		void setSpeed(unsigned int _speed);
		void getSpeed();
};

#endif