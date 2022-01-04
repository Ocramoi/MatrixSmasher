#include "./CutScene.hpp"

using std::make_pair;
using std::make_shared;
using std::max;

CutScene::CutScene(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
			) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
}

void CutScene::appendSprite(Animation& newSprite, const bool shouldLoop) {
    sprites.push_back(make_pair(newSprite, shouldLoop));
}

void CutScene::appendSprite(SpriteSheet& spriteSheet,  
                        raylib::Vector2 startPos,                    
                        const bool shouldLoop
                    ) {
    auto newSprite = make_shared<Animation>(spriteSheet, frameRate, startPos);
    sprites.pushBack(make_pair(newSprite, shouldLoop));
}

void CutScene::appendSprite(SpriteSheet& spriteSheet,  
                        raylib::Vector2 startPos,                    
                        float scale,      
                        const bool shouldLoop
                    ) {
    spriteSheet.scale(scale);
    auto newSprite = make_shared<Animation>(spriteSheet, frameRate, startPos);
    sprites.pushBack(make_pair(newSprite, shouldLoop));
}

void CutScene::setFrameRate(unsigned int _frameRate) {
    frameRate = _frameRate;
    for (auto e : sprites) e.first->setFrameSpeed(frameRate);
}

void CutScene::init() {
    unsigned int maxFrames{0};
    for (const auto& e : sprites) {
        if (!e.second) maxFrames = max(e.first->getSprite().getAmntFrames(), maxFrames);
        e.first->startLoop(e.second);
    }

    thread([changeState] () {
        this_thread::sleep_for(chrono::seconds(maxFrames*1.f / frameRate + 1.f));
        changeState = true;
    }).detach();

    for (auto& e : sprites) drawStatic.push_back(e.first);
}

void CutScene::draw();