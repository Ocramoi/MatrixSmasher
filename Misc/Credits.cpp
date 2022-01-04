#include "./Credits.hpp"

using std::make_pair;
using std::make_shared;

Credits::Credits(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
			) {
	win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
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
        _toggleScene(this);
    });
    
    drawStatic->push_back(sprite.first);
    drawStatic->push_back(back);
};

void Credits::draw() {}