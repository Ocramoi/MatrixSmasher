#include "./CutScene.hpp"

using std::make_pair;
using std::make_shared;
using std::max;
using std::move;
using std::lock_guard;

/**
 * @brief Construct a new Cut Scene:: Cut Scene object
 * 
 * @param _win Window of the corresponding scene.
 * @param _drawStack Vector that draws the stack elements. 
 * @param _drawStatic Vector that draws the stack elements.
 * @param _curScene Pointer to the current scene.
 */
CutScene::CutScene(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
}

/**
 * @brief Appends a new sprite/animation to a cut scene.
 * 
 * @param newSprite new sprite/animation to be appended.
 * @param shouldLoop Wheter it should loop or not.
 */
void CutScene::appendSprite(shared_ptr<Animation>& newSprite, bool shouldLoop) {
    sprites.emplace_back(newSprite, shouldLoop);
}

/**
 * @brief Appends a new sprite/animation to a cut scene.
 * 
 * @param spriteSheet Sprite sheet of the sprite/animation.
 * @param startPos Starting position of the animation.
 * @param shouldLoop  Wheter it should loop or not.
 */
void CutScene::appendSprite(
    SpriteSheet& spriteSheet,
    raylib::Vector2 startPos,
    const bool shouldLoop
) {
    auto newSprite = make_shared<Animation>(spriteSheet, frameRate, startPos);
    sprites.push_back(make_pair(newSprite, shouldLoop));
}

/**
 * @brief Appends a new sprite/animation to a cut scene.
 * 
 * @param spriteSheet Sprite sheet of the sprite/animation.
 * @param startPos Starting position of the animation.
 * @param scale Scale of the spriteshet.
 * @param shouldLoop  Wheter it should loop or not.
 */
void CutScene::appendSprite(
    SpriteSheet& spriteSheet,
    raylib::Vector2 startPos,
    float scale,
    const bool shouldLoop
) {
    spriteSheet.scale(scale);
    auto newSprite = make_shared<Animation>(spriteSheet, frameRate, startPos);
    sprites.push_back(make_pair(newSprite, shouldLoop));
}

/**
 * @brief Changes the frame rate of the cutscene.
 * 
 * @param _frameRate new frame rate of all animaitons/sprites
 * in the cutsene.
 */
void CutScene::setFrameRate(unsigned int _frameRate) {
    frameRate = _frameRate;
    for (auto e : sprites) e.first->setframeRate(frameRate);
}


/**
 * @brief Initializes all components of the CutScene scene
 * and puts them on the drawStack and drawStatic vector if
 * necessary.
 * 
 */
void CutScene::init() {
    for (auto& e : sprites) {
        if (!e.second) maxFrames = max(e.first->getSprite().getAmntFrames(), maxFrames);
        e.first->startLoop(e.second);
    }

    for (auto& e : sprites) drawStatic->push_back(e.first);
}

/**
 * @brief Sets the credits animation thread in order to 
 * change the scene after the sprite is done playing.
 * 
 */
void CutScene::setThread() {
    changeScene = thread([&] () {
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<long>(floor(maxFrames*1.f / frameRate + 1.f)) + 2));
        drawStatic->clear();
        nextScene.swap(*curScene);
        (*curScene)->init();
    });
}

void CutScene::draw() {}

CutScene::~CutScene() {}
