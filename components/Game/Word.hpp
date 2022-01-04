#ifndef WORD_H_
#define WORD_H_

#include <iostream>

#include "../../raylib-cpp/include/raylib-cpp.hpp"

using std::string;

class Word {
	private:
		float speed{10.f},
			rotation{0.f};
		raylib::Vector2 pos{-1.f, -1.f};
		string text{""};
	public:
		void setSpeed(float _speed);
		void fall();
		void fall(float step);
		Word(const string &_word);
		raylib::Vector2 getPos();
		bool input(char key);
};

#endif