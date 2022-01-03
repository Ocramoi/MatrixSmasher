#include "Animation.hpp"

#include <chrono>

using std::max;
using std::move;

Animation::Animation(SpriteSheet _sprite, unsigned int _frameSpeed, raylib::Vector2 _pos) {
    sprite = _sprite;
    frameSpeed = _frameSpeed;
    pos = _pos;
    frameRec.SetSize({sprite.getFrameWidth(), sprite.getFrameHeight()});
}

void Animation::startLoop() {
    startLoop(true);
}

void Animation::startLoop(bool shouldLoop) {
    animHandler = thread([&] () {
        std::this_thread::sleep_for(std::chrono::milliseconds(frameSpeed));
        auto qnt = sprite.getAmntFrames();
        // std::cout << qnt << std::endl;
        if (frameCounter >= qnt) {
            if (!shouldLoop) this->stopLoop();
        }
        frameCounter++;
        if (qnt != 0)
            frameCounter %= qnt;
    });
    animHandler.detach();
}

void Animation::stopLoop() {
    animHandler.~thread();
}

SpriteSheet Animation::getSprite() {
    return sprite;
}

void Animation::setSprite(SpriteSheet _sprite) {
    sprite = _sprite;
    frameRec.SetSize({sprite.getFrameWidth(), sprite.getFrameHeight()});
}

raylib::Vector2 Animation::getPosition() {
    return pos;
}

void Animation::setPosition(raylib::Vector2 v) {
    pos = v;
}

string Animation::getText() {
    return "ablabla";
}

void Animation::setText(const string& newText) {
    return;
}

optional<bool> Animation::checkCollision(const raylib::Vector2 &point) {
    if (point.x < 0 || point.y < 0) return {};
    return boundingBox.CheckCollision(point);
}

void Animation::click() {
    return;
}

void Animation::interact() {
    click();
}

void Animation::setClick(const function<void()>& f) {
    return;
}

void Animation::draw() {
    if (sprite.getAlignment() == VERTICAL)
        frameRec.SetPosition({0.0f, frameCounter * sprite.getFrameWidth()});
    else 
        frameRec.SetPosition({static_cast<float>(frameCounter) * sprite.getFrameHeight(), 0.0f});

    // DrawTexture(sprite.getTexture(), pos.x, pos.y, WHITE);    
    DrawTextureRec(sprite.getTexture(), frameRec, pos, WHITE);
}

float Animation::getHeight() {
    return sprite.getSheet().GetHeight()*1.f;
}