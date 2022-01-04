#ifndef WORD_H_
#define WORD_H_

#include <iostream>

#include "../../raylib-cpp/include/raylib-cpp.hpp"

using std::string;

class Word {
	private:
		float speed{10.f},
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
		void fall();
		void fall(float step);
		raylib::Vector2 getPos();
		bool input(char key);
		void draw();
};

#endif