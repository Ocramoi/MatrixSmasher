#include "./Word.hpp"

Word::Word(const string& _word, const raylib::Window& win) {
	constexpr auto rotationSpan = 30.f,
		minRotation = -rotationSpan,
		maxRotation = rotationSpan;
	constexpr int wordPadding{20};
	text = _word;
	speed /= text.size();
	rotation = (1.f*rand()/RAND_MAX) * (maxRotation - minRotation) + minRotation;
	textSize = raylib::MeasureText(text, fontSize);
	height = abs(sin(rotation)*textSize);
	width = abs(cos(rotation)*textSize);
	pos = raylib::Vector2{
		(1.f*rand()/RAND_MAX)*(win.GetWidth() - 2*wordPadding - width) + wordPadding,
		-height
	};
}

void Word::setSpeed(float _speed) {
	if (_speed < 0) return;
	speed = _speed;
}

float Word::getSpeed() {
	return speed;
}

void Word::fall() {
	pos.y += speed;
}

void Word::fall(float step) {
	if (step >= 0) pos.y += step;
	else fall();
}

pair<bool, bool> Word::input(char key) {
	if (key == 0) return { false, false };
	auto hit = key == text.front();
	if (hit) text.erase(text.begin());
	return { hit, text.empty() };
}

void Word::draw() {
	raylib::DrawTextPro(
		raylib::Font{GetFontDefault()},
		text,
		pos,
		raylib::Vector2{0, 0},
		rotation,
		fontSize,
		fontSpacing,
		fontColor
	);
}

raylib::Vector2 Word::getPos() {
	return pos;
}
