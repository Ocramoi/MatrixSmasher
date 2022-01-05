#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <iostream>
#include <optional>
#include <functional>
#include <thread>

#include "../raylib-cpp/include/raylib-cpp.hpp"
#include "SpriteSheet.hpp"
#include "UIElement.cpp"

using std::string;
using std::optional;
using std::function;
using std::thread;

class Animation : public UIElement {
    private:
        SpriteSheet sprite;
        raylib::Vector2 pos{-1, -1};
        unsigned int frameSpeed{1}, 
                     frameCounter{0};
        raylib::Rectangle frameRec{ 0.0f, 0.0f, 0.0f, 0.0f};
        raylib::Rectangle boundingBox{-1};
        thread animHandler;   
    public:
        Animation() {}
        ~Animation() {
            animHandler.~thread();
        };
        void startLoop();
        void startLoop(bool shouldLoop);
        void stopLoop();
        Animation(SpriteSheet _sprite, unsigned int _frameSpeed, raylib::Vector2 _pos);
        SpriteSheet getSprite();
        void setSprite(SpriteSheet _sprite);
        raylib::Vector2 getPosition();
        void setPosition(raylib::Vector2 v);
        string getText();
        void setText(const string& newText);
        void setFrameSpeed(unsigned int _frameSpeed);
        optional<bool> checkCollision(const raylib::Vector2 &point);
        void click();
        void interact();
        void setClick(const function<void()>& f);
        float getHeight();
        unsigned int getWidth();
        void draw();
};

#endif // ANIMATION_H_
