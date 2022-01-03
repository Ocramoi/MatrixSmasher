#ifndef UIELEMENT_H_
#define UIELEMENT_H_

#include <iostream>
#include <optional>

#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::string;
using std::optional;

class UIElement {
    public:
        virtual ~UIElement() = default;
        virtual string getText() = 0;
        virtual void setText(const string&) = 0;
        virtual raylib::Vector2 getPosition() = 0;
        virtual void setPosition(raylib::Vector2) = 0;
        virtual optional<bool> checkCollision(const raylib::Vector2&) = 0;
        virtual float getHeight() = 0;
        virtual void interact() = 0;
        virtual void draw() = 0;
};

#endif // UIELEMENT_H_
