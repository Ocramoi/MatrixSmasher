#include "./Word.hpp"

Word::Word(const string& _word, const raylib::Window& win) {
	constexpr auto rotationSpan = 45.f, 
		minRotation = 2*PI*(-rotationSpan/360), 
		maxRotation = 2*PI*(rotationSpan/360);
	text = _word;
	rotation = (1.f*rand()/RAND_MAX) * (maxRotation - minRotation) + minRotation;
	textSize = raylib::MeasureText(text, fontSize);
	height = abs(sin(rotation))*textSize;
	width = abs(cos(rotation))*textSize;
	pos = raylib::Vector2{
		(1.f*rand()/RAND_MAX) * win.GetWidth(),
		-height
	};
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

raylib::Vector2 Word::getPos() {
	return pos;
}