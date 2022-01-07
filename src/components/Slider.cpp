#include "Slider.hpp"

using std::max;
using std::move;

constexpr float SLIDER_PADDING{5};
constexpr float SLIDER_BOX{10};
constexpr float SLIDER_BOX_PADDING{2};

/**
 * @brief Construct a new Slider:: Slider object
 * 
 * @param _text Text of the slider.
 * @param _maxValue Max value of the slider.
 * @param _pos Window position of the slider.
 */
Slider::Slider(const string& _text, unsigned int _maxValue, raylib::Vector2 _pos) {
    pos = move(_pos);
    width = raylib::MeasureText(_text, fontSize);
    maxValue = _maxValue;
    setText(_text);
    setClick([]() { return; });
}

/**
 * @brief Construct a new Slider:: Slider object
 * 
 * @param _text Text of the slider.
 * @param _maxValue Max value of the slider.
 * @param _pos Window position of the slider.
 * @param _width Predefined Width of the slider.
 */
Slider::Slider(const string& _text, unsigned int _maxValue, raylib::Vector2 _pos, unsigned int _width) {
    pos = move(_pos);
    width = _width;
    maxValue = _maxValue;
    setText(_text);
}

/**
 * @brief Returns the position of a slider.
 * 
 * @return raylib::Vector2 Slider position
 */
raylib::Vector2 Slider::getPosition() {
    return pos;
}

/**
 * @brief Changes the position of a slider on screen.
 * 
 * @param v New position.
 */
void Slider::setPosition(raylib::Vector2 v) {
    pos = v;
    setText(text);
}

/**
 * @brief Gets the text of a slider.
 * 
 * @return `string` slider's text. 
 */
string Slider::getText() {
    return text;
}

/**
 * @brief Sets the text of the slider in place 
 * and defines its bounding box.
 * 
 * @param newText Text to be set in place.
 */
void Slider::setText(const string& newText) {
    text = newText;
    auto text_lines = Logic::clampText(text, width, fontSize);
    formatedText = text_lines.first;
    height = text_lines.second;

    auto totalWidth = width + (maxValue-1.f)*(SLIDER_BOX + SLIDER_BOX_PADDING) +  2.f*SLIDER_PADDING,
        totalHeight = height*fontSize + (height - 1.f)*(fontSize*0.5f) + 2.f*SLIDER_PADDING + SLIDER_BOX + 2.f*SLIDER_BOX_PADDING;
    boundingBox = raylib::Rectangle(
        {max(0.f, pos.x), max(0.f, pos.y)},
        {totalWidth, totalHeight}
    );
}

/**
 * @brief Sets the onClick function of a slider.
 * 
 * @param f Function to be activated onClick.
 */
void Slider::setClick(const function<void()>& f) {
    onClick = f;
}

/**
 * @brief Draws a slider object;
 * 
 */
void Slider::draw() {
    boundingBox.Draw(background); 
    boundingBox.DrawLines(border, 2);
    
    raylib::DrawText(formatedText, pos.x + SLIDER_PADDING, pos.y + SLIDER_PADDING, fontSize, fontColor);
     
    auto posY = pos.y + (height*fontSize) + (height - 1.f)*(fontSize*0.5f) + SLIDER_PADDING + SLIDER_BOX_PADDING,
        boxesTotal = maxValue*(SLIDER_BOX) + max((1.f*maxValue - 1)*SLIDER_BOX_PADDING, 0.f),
        posX = pos.x + width/2.f - boxesTotal/2.f;

    for (decltype(maxValue) i = 0; i < maxValue; i++) {
        if (i < curValue) DrawRectangle(posX, posY , SLIDER_BOX, SLIDER_BOX, RED);
        DrawRectangleLines(posX, posY, SLIDER_BOX, SLIDER_BOX, MAROON);
        posX += SLIDER_BOX + SLIDER_BOX_PADDING;
    }
}

/**
 * @brief Checks if there is colision of the bounding box
 * of the slider with another point
 * 
 * @param point Point to check if there is collision
 * @return `optional<bool>`  `none` if the point is invalid, 
 * `true` if there is collision and `false` otherwise.
 * 
 */
optional<bool> Slider::checkCollision(const raylib::Vector2 &point) {
    if (point.x < 0 || point.y < 0) return {};
    return boundingBox.CheckCollision(point);
}

/**
 * @brief Returns the height of the slider
 * 
 * @return `float` slider's height. 
 */
float Slider::getHeight() {
    return boundingBox.GetHeight();
}

/**
 * @brief Returns the width of the slider
 * 
 * @return `float` slider's width. 
 */
unsigned int Slider::getWidth() {
    return width;
}

/**
 * @brief Changes the state and background
 * color of a slider onClick.
 * 
 */
void Slider::click() {
    state = !state;
    if (state) setBackground(onFocusColor);
    else setBackground(baseColor);
}

/**
 * @brief Defines the interaction of a slider.
 * First executes if onClick function and then changes
 * it state.
 * 
 */
void Slider::interact() {
    click();
    onClick();
}

/**
 * @brief Change the colors of a slider.
 * 
 * @param _border New border color.
 * @param _background New background color.
 * @param _fontColor New font color.
 */
void Slider::setColors(raylib::Color _border, raylib::Color _background, raylib::Color _fontColor) {
    border = _border;
    background = _background;
    fontColor = _fontColor;
}

/**
 * @brief Changes the font color of a slider.
 * 
 * @param _fontColor New font color.
 */
void Slider::setFontColor(raylib::Color _fontColor) {
    fontColor = _fontColor;
}

/**
 * @brief Changes the background color of a slider.
 * 
 * @param _background New background color.
 */
void Slider::setBackground(raylib::Color _background) {
    background = _background;
}

/**
 * @brief Changes the border color of a slider.
 * 
 * @param _border New border color.
 */
void Slider::setBorder(raylib::Color _border) {
    border = _border;
}

/**
 * @brief Increased the value of the slider,
 * until it reaches the maxValue.
 * 
 */
void Slider::increase() {
    if (curValue < maxValue) curValue++;
}

/**
 * @brief Decreases the value of the slider,
 * until it reaches the zero.
 * 
 */
void Slider::decrease() {
    if (curValue > 0) curValue--;
}

/**
 * @brief Returns the state of the slider.
 * 
 * @return `true` if its on focus/selected.
 * @return `false` if its not selected.
 */
bool Slider::getState() {
    return state;
}

/**
 * @brief Sets the curValue of the slider
 * as long as its smaller than the max value.
 * 
 * @param val New current value of the slider.
 */
void Slider::setValue(unsigned int val) {
    curValue = std::min(maxValue, val);
}

/**
 * @brief Returns the current value of the slider.
 * 
 * @return `unsigned int` slider's current value. 
 */
unsigned int Slider::getValue() {
    return curValue;
}
