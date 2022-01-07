#include "./Options.hpp"
#include "./Menu.hpp"

using std::make_pair;
using std::make_shared;
using std::max;

Options::Options(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
}

void Options::setSprite(shared_ptr<Animation>& _sprite) {
    setSprite(_sprite, true);
}

void Options::setSprite(shared_ptr<Animation>& _sprite, const bool shouldLoop) {
    sprite = make_pair(_sprite, shouldLoop);
}

void Options::_toggleScene(Options* _c) { _c->changeState = true; }

void Options::init() {
    constexpr int maxButtonWidth = 200;

    SpriteSheet bg{raylib::Image{"../resources/spritesheets/background.png"}, 1, HORIZONTAL};
    auto bgScale = max(1.f*win->GetWidth()/bg.getFrameWidth(), 1.f*win->GetHeight()/bg.getFrameHeight());
    bg.scale(bgScale);
    sprite = { make_shared<Animation>(bg, 12U, raylib::Vector2{0, 0}), false };

    raylib::Vector2 winSize = win->GetSize(),
        midSize = winSize.Divide(2.f);

    speed = make_shared<Slider>("Difficulty", 3U, midSize, maxButtonWidth);
    speed->setBorder(raylib::Color::DarkGreen());
    speed->setFontColor(raylib::Color::DarkGreen());
    speed->setClick([&] () {
        keyboardCapture = thread(
            [&]() {
                while (true){
                    auto key = GetKeyPressed();
                    if (!key) continue;
                    if (key == KEY_RIGHT) speed->increase();
                    else if (key == KEY_LEFT) speed->decrease();
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        );
        keyboardCapture.detach();
    });

    sprite.first->startLoop(sprite.second);

    auto back = make_shared<Button>("Back", raylib::Vector2 {10, 10});
    back->setBorder(raylib::Color::DarkGreen());
    back->setFontColor(raylib::Color::DarkGreen());
    back->setClick([&] () {
        drawStatic->clear();
        shared_ptr<Scene> temp = make_shared<Menu>(win, drawStack, drawStatic, *curScene);
        temp.swap(*curScene);
        (*curScene)->init();
    });

    drawStatic->push_back(sprite.first);
    drawStatic->push_back(back);
};

void Options::draw() {
    drawStack->push_back(speed);
}

Options::~Options() {
    keyboardCapture.~thread();
}
