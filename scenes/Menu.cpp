#include "./Menu.hpp"

using std::make_shared;
using std::max;

Menu::Menu(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; 
    drawStack = _drawStack; 
    drawStatic = _drawStatic;
    curScene = _curScene;
}

void _configButtons(shared_ptr<Button>& button) {
    button->setBorder(raylib::Color::DarkGreen());
    button->setFontColor(raylib::Color::DarkGreen());
}

template<class... UI>
void _configButtons(shared_ptr<Button>& button, UI... buttons) {
    _configButtons(button);
    _configButtons(buttons...);
}

void Menu::init() {
    constexpr int maxButtonWidth = 100;
    raylib::Vector2 winSize = win->GetSize(),
        midSize = winSize.Divide(2.f);

    auto start = make_shared<Button>("Start", midSize, maxButtonWidth), 
        options = make_shared<Button>("Credits", midSize, maxButtonWidth),
        exit = make_shared<Button>("Exit", midSize, maxButtonWidth);
    _configButtons(start, options, exit);

    start->setClick([&] () {
        shared_ptr<CutScene> _cut =  make_shared<CutScene>(win, drawStack, drawStatic, curScene);
        shared_ptr<Scene> sCut = _cut;
        sCut.swap(curScene);
        SpriteSheet boss{raylib::Image{"./resources/spritesheets/boss.png"}, 4, HORIZONTAL},
            bubble{raylib::Image{"./resources/spritesheets/bubble.png"}, 30, HORIZONTAL},
            bg{raylib::Image{"./resources/spritesheets/background.png"}, 30, HORIZONTAL};
        auto bgScale = max(1.f*winSize.x/bg.getFrameWidth(), 1.f*winSize.y/bg.getFrameHeight());

        _cut->appendSprite(boss, midSize, 0.7, true);
        _cut->appendSprite(bubble, midSize, 0.5, false);
        _cut->appendSprite(bg, raylib::Vector2 {0, 0}, bgScale, true);

        shared_ptr<Game> temp = make_shared<Game>(win, drawStack, drawStatic, curScene);
        _cut->setScene(temp);
        _cut->setFrameRate(12U);

        curScene->init();
    });

    options->setClick([&] () {
        shared_ptr<Scene> temp = make_shared<Credits>(win, drawStack, drawStatic, curScene);
        temp->init();
        temp.swap(curScene);
    });

    exit->setClick([&] () {
        curScene = nullptr;
    });

    for (auto &el : Logic::setPosStack(10, VERTICAL, start, options, exit))
        drawStatic->push_back(el);
}

void Menu::draw() {}

Menu::~Menu() {
    drawStatic->clear();
}