#include "Animation.hpp"

using std::max;
using std::move;

Animation::Animation(SpriteSheet _sprite, float _duration, raylib::Vector2 _pos) {
    sprite = _sprite;
    duration = _duration;
    pos = move(_pos);
}

Animation::Animation(SpriteSheet _sprite, float _duration, unsigned int _frameSpeed, raylib::Vector2 _pos) {
    sprite = _sprite;
    duration = _duration;
    frameSpeed = _frameSpeed;
    pos = move(_pos);
}

SpriteSheet Animation::getSprite() {
    return sprite;
}

void Animation::setSprite(SpriteSheet _sprite) {
    sprite = _sprite;
}

raylib::Vector2 Animation::getPosition() {
    return pos;
}

void Animation::setPosition(raylib::Vector2 v) {
    pos = move(v);
}

string Animation::getText() {
    return "ablabla";
}

void Animation::setText(const string& newText) {
    ;
}

optional<bool> Animation::checkCollision(const raylib::Vector2 &point) {
    if (point.x < 0 || point.y < 0) return {};
    return boundingBox.CheckCollision(point);
}

void Animation::click() {
    ;
}

void Animation::interact() {
    ;
}

void Animation::setClick(const function<void()>& f) {
    ;
}

void Animation::draw() {
    ;
}
