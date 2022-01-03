#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <iostream>
#include <optional>
#include <functional>

#include "../raylib-cpp/include/raylib-cpp.hpp"
#include "SpriteSheet.hpp"
#include "UIElement.cpp"

using std::string;
using std::optional;
using std::function;

class Animation : public UIElement {
    private:
        SpriteSheet sprite;
        raylib::Vector2 pos{-1, -1};
        float duration{-1};
        unsigned int frameSpeed{1};
        raylib::Rectangle boundingBox{-1};
    public:
        Animation() {}
        Animation(SpriteSheet _sprite, float _duration, raylib::Vector2 _pos);
        Animation(SpriteSheet _sprite, float _duration, unsigned int _frameSpeed, raylib::Vector2 _pos);
        SpriteSheet getSprite();
        void setSprite(SpriteSheet _sprite);
        raylib::Vector2 getPosition();
        void setPosition(raylib::Vector2 v);
        string getText();
        void setText(const string& newText);
        optional<bool> checkCollision(const raylib::Vector2 &point);
        void click();
        void interact();
        void setClick(const function<void()>& f);
        void draw();
};

#endif // ANIMATION_H_
