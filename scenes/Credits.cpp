#include "./Credits.hpp"

using std::make_pair;
using std::make_shared;

Credits::Credits(
    shared_ptr<raylib::Window>& _win,
	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
	shared_ptr<Scene>& _curScene
) {
	win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = _curScene;
    SpriteSheet boss{raylib::Image{"./resources/spritesheets/boss.png"}, 4, HORIZONTAL};
    sprite = { make_shared<Animation>(boss, 12U, raylib::Vector2{0, 0}), true };
}

void Credits::setSprite(shared_ptr<Animation>& _sprite) {
    setSprite(_sprite, true);
}

void Credits::setSprite(shared_ptr<Animation>& _sprite, const bool shouldLoop) {
    sprite = make_pair(_sprite, shouldLoop);
}

void Credits::_toggleScene(Credits* _c) { _c->changeState = true; }

void Credits::init() {
    sprite.first->startLoop(sprite.second);

    auto back = make_shared<Button>("Back", raylib::Vector2 {0, 0});
    back->setBorder(raylib::Color::DarkGreen());
    back->setFontColor(raylib::Color::DarkGreen());
    back->setClick([&] () {
        curScene = make_shared<Menu>(win, drawStack, drawStatic, curScene);
    });
    
    drawStatic->push_back(sprite.first);
    drawStatic->push_back(back);
};

void Credits::draw() {}