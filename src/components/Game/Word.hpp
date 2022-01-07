#ifndef WORD_H_
#define WORD_H_

#include <utility>
#include <iostream>

#include "../../raylib-cpp/include/raylib-cpp.hpp"

using std::string;
using std::pair;

class Word {
	private:
		float speed{15.f},
			rotation{0.f},
			fontSize{20.f},
			fontSpacing{2.f},
			height{-1.f},
			width{-1.f},
			textSize{-1.f};
		raylib::Color fontColor{raylib::Color::Black()};
		raylib::Vector2 pos{-1.f, -1.f};
		string text{""};
	public:
		Word(const string &_word, const raylib::Window& win);
		void setSpeed(float _speed);
		float getSpeed();
		void fall();
		void fall(float step);
		raylib::Vector2 getPos();
		pair<bool, bool> input(char key);
		void draw();
};

#endif
