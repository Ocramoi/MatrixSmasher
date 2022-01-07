#include "./CutScene.hpp"

using std::make_pair;
using std::make_shared;
using std::max;
using std::move;
using std::lock_guard;

CutScene::CutScene(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
}

void CutScene::appendSprite(shared_ptr<Animation>& newSprite, bool shouldLoop) {
    sprites.emplace_back(newSprite, shouldLoop);
}

void CutScene::appendSprite(
    SpriteSheet& spriteSheet,
    raylib::Vector2 startPos,
    const bool shouldLoop
) {
    auto newSprite = make_shared<Animation>(spriteSheet, frameRate, startPos);
    sprites.push_back(make_pair(newSprite, shouldLoop));
}

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

void CutScene::setFrameRate(unsigned int _frameRate) {
    frameRate = _frameRate;
    for (auto e : sprites) e.first->setframeRate(frameRate);
}

void CutScene::init() {
    for (auto& e : sprites) {
        if (!e.second) maxFrames = max(e.first->getSprite().getAmntFrames(), maxFrames);
        e.first->startLoop(e.second);
    }

    for (auto& e : sprites) drawStatic->push_back(e.first);
}

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
