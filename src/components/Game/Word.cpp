#include "./Word.hpp"

/**
 * @brief Construct a new Word:: Word object
 * 
 * @param _word Text of the given word.
 * @param win Window where the word will be displayed.
 */
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

/**
 * @brief Modifies the falling speed of a word.
 * 
 * @param _speed New speed of the word.
 */
void Word::setSpeed(float _speed) {
	if (_speed < 0) return;
	speed = _speed;
}

/**
 * @brief Returns the falling speed of a word.
 * 
 * @return Current speed of an word.
 */
float Word::getSpeed() {
	return speed;
}

/**
 * @brief Updates the ypos of the falling word
 * using its speed.
 */
void Word::fall() {
	pos.y += speed;
}

/**
 * @brief Updates the ypos of the falling word
 * using an step instead of its on speed.
 *
 * @param step Value used to increment 
 * the ypos.
 */
void Word::fall(float step) {
	if (step >= 0) pos.y += step;
	else fall();
}

/**
 * @brief Checks if a given char matches the current  
 * begning char of the word and if so,
 * consumes it changing the begning char.
 *
 * @param key Current pressed char.
 * @return returns a pair of bools; the first 
 * corresponds if the key pressed was correct
 * and the second the text has been emptied.
*/
pair<bool, bool> Word::input(char key) {
	if (key == 0) return { false, false };
	auto hit = key == text.front();
	if (hit) text.erase(text.begin());
	return { hit, text.empty() };
}

/**
 * @brief Draws a falling word on screen. 
 */
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

/**
 * @brief Returns the current position of
 * a word on screen. 
 *
 * @return Current pos on screen
 */
raylib::Vector2 Word::getPos() {
	return pos;
}
