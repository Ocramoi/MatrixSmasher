#include "Button.hpp"

using std::max;
using std::move;

constexpr float BUTTON_PADDING{5};

/**
 * @brief Construct a new Button:: Button object
 * 
 * @param _text  Text to be displayed on the button.
 * @param _pos Position of the button on screen.
 */
Button::Button(const string& _text, raylib::Vector2 _pos) {
    pos = move(_pos);
    width = raylib::MeasureText(_text, fontSize);
    setText(_text);
    setClick([]() { return; });
}

/**
 * @brief Construct a new Button:: Button object
 * 
 * @param _text  Text to be displayed on the button.
 * @param _pos Position of the button on screen.
 * @param _width Predefined width of the button. 
 */
Button::Button(const string& _text, raylib::Vector2 _pos, unsigned int _width) {
    pos = move(_pos);
    width = _width;
    setText(_text);
}

/**
 * @brief Gets the position of the button on screen.
 * 
 * @return `raylib::Vector2` position of the button.
 */
raylib::Vector2 Button::getPosition() {
    return pos;
}

/**
 * @brief Sets the position of the button on screen.
 * 
 * @param v New position of the button.
 */
void Button::setPosition(raylib::Vector2 v) {
    pos = v;
    setText(text);
}

/**
 * @brief Gets the text of the button.
 * 
 * @return `string` button's text.
 */
string Button::getText() {
    return text;
}

/**
 * @brief Sets the text to be displayed on a button.
 * 
 * @param newText New text of the button.
 */
void Button::setText(const string& newText) {
    text = newText;
    auto text_lines = Logic::clampText(text, width, fontSize);
    formatedText = text_lines.first;
    height = text_lines.second;
    boundingBox = raylib::Rectangle(
        { max(0.f, pos.x - BUTTON_PADDING), max(0.f, pos.y - BUTTON_PADDING) },
        { width + 2*BUTTON_PADDING, height*fontSize + (height - 1)*(fontSize*0.5f) + 2*BUTTON_PADDING });
}

/**
 * @brief Sets the click of a button.
 * 
 * @param f Function to be activated on click
 * of the button.
 */
void Button::setClick(const function<void()>& f) {
    onClick = f;
}

/**
 * @brief Draws a button on screen.
 * 
 */
void Button::draw() {
    boundingBox.Draw(background); 
    boundingBox.DrawLines(border, 2);
    raylib::DrawText(formatedText, pos.x, pos.y, fontSize, fontColor);
}

/**
 * @brief Checks if there is colision of the bounding box
 * of the button with another point
 * 
 * @param point Point to check if there is collision
 * @return `optional<bool>`  `none` if the point is invalid, 
 * `true` if there is collision and `false` otherwise.
 * 
 */
optional<bool> Button::checkCollision(const raylib::Vector2 &point) {
    if (point.x < 0 || point.y < 0) return {};
    return boundingBox.CheckCollision(point);
}

/**
 * @brief Returns the height of a button
 * 
 * @return `float` button's height. 
 */
float Button::getHeight() {
    return boundingBox.GetHeight();
}

unsigned int Button::getWidth() {
    return width;
}

/**
 * @brief Changes the state of a button on click.
 * 
 */
void Button::click() {
    state = !state;
}

/**
 * @brief Defines the interaction of a button.
 * First executes if onClick function and then changes
 * it state.
 * 
 */
void Button::interact() {
    onClick();
    click();
}

/**
 * @brief Change the colors of a button.
 * 
 * @param _border New border color.
 * @param _background New background color.
 * @param _fontColor New font color.
 */
void Button::setColors(raylib::Color _border, raylib::Color _background, raylib::Color _fontColor) {
    border = _border;
    background = _background;
    fontColor = _border;
}

/**
 * @brief Changes the font color of a button.
 * 
 * @param _fontColor New font color.
 */
void Button::setFontColor(raylib::Color _fontColor) {
    fontColor = _fontColor;
}

/**
 * @brief Changes the background color of a button.
 * 
 * @param _background New background color.
 */
void Button::setBackground(raylib::Color _background) {
    background = _background;
}

/**
 * @brief Changes the border color of a button.
 * 
 * @param _border New border color.
 */
void Button::setBorder(raylib::Color _border) {
    border = _border;
}
