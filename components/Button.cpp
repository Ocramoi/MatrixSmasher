#include "Button.hpp"

using std::max;
using std::move;

constexpr float BUTTON_PADDING{5};

Button::Button(const string& _text, raylib::Vector2 _pos) {
    pos = move(_pos);
    width = raylib::MeasureText(_text, fontSize);
    setText(_text);
    setClick([]() { return; });
}

Button::Button(const string& _text, raylib::Vector2 _pos, unsigned int _width) {
    pos = move(_pos);
    width = _width;
    setText(_text);
}

raylib::Vector2 Button::getPosition() {
    return pos;
}

void Button::setPosition(raylib::Vector2 v) {
    pos = v;
    setText(text);
}

string Button::getText() {
    return text;
}

void Button::setText(const string& newText) {
    text = newText;
    auto text_lines = Logic::clampText(text, width, fontSize);
    formatedText = text_lines.first;
    height = text_lines.second;
    boundingBox = raylib::Rectangle(
        { max(0.f, pos.x - BUTTON_PADDING), max(0.f, pos.y - BUTTON_PADDING) },
        { width + 2*BUTTON_PADDING, height*fontSize + (height - 1)*(fontSize*0.5f) + 2*BUTTON_PADDING });
}

void Button::setClick(const function<void()>& f) {
    onClick = f;
}

void Button::draw() {
    boundingBox.Draw(background); boundingBox.DrawLines(border, 2);
    raylib::DrawText(formatedText, pos.x, pos.y, fontSize, fontColor);
}

optional<bool> Button::checkCollision(const raylib::Vector2 &point) {
    if (point.x < 0 || point.y < 0) return {};
    return boundingBox.CheckCollision(point);
}

float Button::getHeight() {
    return boundingBox.GetHeight();
}

unsigned int Button::getWidth() {
    return width;
}

void Button::click() {
    state = !state;
}

void Button::interact() {
    onClick();
    click();
}

void Button::setColors(raylib::Color _border, raylib::Color _background, raylib::Color _fontColor) {
    border = _border;
    background = _background;
    fontColor = _border;
}

void Button::setFontColor(raylib::Color _fontColor) {
    fontColor = _fontColor;
}

void Button::setBackground(raylib::Color _background) {
    background = _background;
}

void Button::setBorder(raylib::Color _border) {
    border = _border;
}
