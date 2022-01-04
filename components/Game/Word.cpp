#include "./Word.hpp"

Word::Word(const string& _word) {
	text = _word;
}

void Word::setSpeed(float _speed) {
	if (_speed < 0) return;
	speed = _speed;
}

void Word::fall() {
	pos.y += speed;
}

void Word::fall(float step) {
	if (step >= 0) pos.y += step;
	else fall();
}

bool Word::input(char key) {
	if (key == 0 || key != text.front()) return false;
	text.erase(text.begin());
	return text.empty();
}

void Word::draw() {
	raylib::DrawTextPro(
		raylib::Font{GetFontDefault()},
		text,
		pos,
		pos,
		rotation,
		fontSize,
		fontSpacing,
		fontColor
	);
}