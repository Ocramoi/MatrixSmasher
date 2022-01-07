#ifndef SLIDER_H_
#define SLIDER_H_

#include <iostream>
#include <optional>
#include <functional>

#include "../raylib-cpp/include/raylib-cpp.hpp"
#include "UIElement.cpp"

#include "../components/Logic.hpp"

using std::string;
using std::pair;
using std::optional;
using std::function;

class Slider : public UIElement {
    private:
        string text{""},
            formatedText{""};
        raylib::Vector2 pos{-1, -1};
        float height{-1};
        unsigned int width{0},
            fontSize{20};
        raylib::Rectangle boundingBox{-1};
        raylib::Color border{raylib::Color::Black()},
            baseColor{raylib::Color::White()},
            background{baseColor},
            onFocusColor{raylib::Color::LightGray()},
            fontColor{raylib::Color::Black()};
        bool state{false};
        function<void(void)> onClick{[]() {}};
        unsigned int maxValue{0},
            curValue{0};
    public:
        Slider(const string& _text, unsigned int _maxValue, raylib::Vector2 _pos);
        Slider(const string& _text, unsigned int _maxValue, raylib::Vector2 _pos, unsigned int _width);
        string getText();
        void setText(const string& newText);
        raylib::Vector2 getPosition();
        void setPosition(raylib::Vector2 v);
        unsigned int getWidth();
        void setWidth(unsigned int w);
        float getHeight();
        pair<raylib::Color, raylib::Color> getColors();
        void setColors(raylib::Color _border, raylib::Color _background, raylib::Color _fontColor);
        void setFontColor(raylib::Color _fontColor);
        void setBackground(raylib::Color _background);
        void setBorder(raylib::Color _border);
        optional<bool> checkCollision(const raylib::Vector2 &point);
        void click();
        void interact();
        void setClick(const function<void()>& f);
        void draw();
        void increase();
        void decrease();
        bool getState();
        unsigned int getValue();
        void setValue(unsigned int val);
};

#endif // SLIDER_H_
