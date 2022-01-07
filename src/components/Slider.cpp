#include "Slider.hpp"

using std::max;
using std::move;

constexpr float SLIDER_PADDING{5};
constexpr float SLIDER_BOX{10};
constexpr float SLIDER_BOX_PADDING{2};


Slider::Slider(const string& _text, unsigned int _maxValue, raylib::Vector2 _pos) {
    pos = move(_pos);
    width = raylib::MeasureText(_text, fontSize);
    maxValue = _maxValue;
    setText(_text);
    setClick([]() { return; });
}

Slider::Slider(const string& _text, unsigned int _maxValue, raylib::Vector2 _pos, unsigned int _width) {
    pos = move(_pos);
    width = _width;
    maxValue = _maxValue;
    setText(_text);
}

raylib::Vector2 Slider::getPosition() {
    return pos;
}

void Slider::setPosition(raylib::Vector2 v) {
    pos = v;
    setText(text);
}

string Slider::getText() {
    return text;
}

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

void Slider::setClick(const function<void()>& f) {
    onClick = f;
}

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

optional<bool> Slider::checkCollision(const raylib::Vector2 &point) {
    if (point.x < 0 || point.y < 0) return {};
    return boundingBox.CheckCollision(point);
}

float Slider::getHeight() {
    return boundingBox.GetHeight();
}

unsigned int Slider::getWidth() {
    return width;
}

void Slider::click() {
    state = !state;
    if (state) setBackground(onFocusColor);
    else setBackground(baseColor);
}

void Slider::interact() {
    click();
    onClick();
}

void Slider::setColors(raylib::Color _border, raylib::Color _background, raylib::Color _fontColor) {
    border = _border;
    background = _background;
    fontColor = _fontColor;
}

void Slider::setFontColor(raylib::Color _fontColor) {
    fontColor = _fontColor;
}

void Slider::setBackground(raylib::Color _background) {
    background = _background;
}

void Slider::setBorder(raylib::Color _border) {
    border = _border;
}

void Slider::increase() {
    if (curValue < maxValue) curValue++;
}

void Slider::decrease() {
    if (curValue > 0) curValue--;
}

bool Slider::getState() {
    return state;
}

void Slider::setValue(unsigned int val) {
    curValue = std::min(maxValue, val);
}

unsigned int Slider::getValue() {
    return curValue;
}
