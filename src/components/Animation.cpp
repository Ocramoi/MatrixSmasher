#include "Animation.hpp"

#include <chrono>

using std::max;
using std::move;

/**
 * @brief Construct a new Animation:: Animation object
 * 
 * @param _sprite  Spritesheet of the corresponding animation.
 * @param _frameRate Frame rate of the animation.
 * @param _pos Position on the current window screen.
 */
Animation::Animation(SpriteSheet _sprite, unsigned int _frameRate, raylib::Vector2 _pos) {
    sprite = _sprite;
    frameRate = 1000/_frameRate;
    pos = _pos;
    frameRec.SetSize({sprite.getFrameWidth(), sprite.getFrameHeight()});
}

/**
 * @brief Starts looping through the 
 * frames of the current animation.
 * 
 */
void Animation::startLoop() {
    startLoop(true);
}

/**
 * @brief Starts looping through the 
 * frames of the current animation.
 * 
 * @param shouldLoop `true` if the sprite sheet
 * frames should repeat once the final frame is reached,
 * `false` otherwise.
 * 
 */
void Animation::startLoop(bool shouldLoop) {
    auto qnt = sprite.getAmntFrames();
    animHandler = thread([&, shouldLoop, qnt] () {
        while (true) {
            {
                unique_lock<mutex> ul(stopMutex);
                conditialAnimation.wait_for(ul, std::chrono::milliseconds(frameRate));
            }
            if (stopThread.load()) break;
            if (++frameCounter >= qnt && !shouldLoop) {
                frameCounter--;
                break;
            }
            frameCounter %= qnt;
        }
    });
}

/**
 * @brief Stops looping the the frames of the animation.
 * 
 */
void Animation::stopLoop() {
    stopThread.store(true);
    conditialAnimation.notify_all();
    animHandler.join();
}

/**
 * @brief Gets the sprite sheet of the animation. 
 * 
 * @return SpriteSheet.
 */
SpriteSheet Animation::getSprite() {
    return sprite;
}

/**
 * @brief Changes the sprite sheet of an animation.
 * 
 * @param _sprite New spritesheet of the animation.
 */
void Animation::setSprite(SpriteSheet _sprite) {
    sprite = _sprite;
    frameRec.SetSize({sprite.getFrameWidth(), sprite.getFrameHeight()});
}

/**
 * @brief Returns the current position of the
 * animation on th screen.
 * 
 * @return Position of the animation. 
 */
raylib::Vector2 Animation::getPosition() {
    return pos;
}

/**
 * @brief Changes the position of a animation.
 * 
 * @param v New position of the animation on screen.
 */
void Animation::setPosition(raylib::Vector2 v) {
    pos = v;
}

string Animation::getText() {
    return "";
}

void Animation::setText(const string& newText) {
    return;
}

/**
 * @brief Checks if there is colision of the bounding box
 * of the animation with another point
 * 
 * @param point Point to check if there is collision
 * @return optional<bool>  `none` if the point is invalid, 
 * `true` if there is collision and `false` otherwise.
 */
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

/**
 * @brief Draws the current frame of the animation.
 * 
 */
void Animation::draw() {
    if (sprite.getAlignment() == VERTICAL)
        frameRec.SetPosition({0.0f, 1.f*frameCounter*sprite.getFrameHeight()});
    else  
        frameRec.SetPosition({static_cast<float>(frameCounter) * sprite.getFrameWidth(), 0.0f});

    DrawTextureRec(sprite.getTexture(), frameRec, pos, WHITE);
}

/**
 * @brief Gets the height of each animation frame.
 * 
 * @return Height of animation frame.
 */
float Animation::getHeight() {
    return frameRec.GetHeight();
}

/**
 * @brief Gets the Width of each animation frame.
 * 
 * @return Width of animation frame.
 */
unsigned int Animation::getWidth() {
    return frameRec.GetWidth();
}

/**
 * @brief Changes the frame rate of an animation.
 * 
 * @param _frameRate New frame rate.
 */
void Animation::setframeRate(unsigned int _frameRate) {
    frameRate = 1000/_frameRate;
}

/**
 * @brief Destroy the Animation:: Animation object
 * 
 */
Animation::~Animation() {
    stopLoop();
}
