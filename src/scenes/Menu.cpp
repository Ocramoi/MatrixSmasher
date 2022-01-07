#include "./Menu.hpp"

using std::make_shared;
using std::max;

Menu::Menu(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vShrdUIEl>& _drawStack,
    shared_ptr<vShrdUIEl>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win;
    drawStack  = _drawStack;
    drawStatic = _drawStatic;
    curScene = &_curScene;
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
    constexpr int maxButtonWidth{100},
        padding{10},
        titlePadding{5*padding};
    raylib::Vector2 winSize{win->GetSize()},
        midSize{winSize.Divide(2.f)},
        butMidPoint{winSize.Subtract(
            raylib::Vector2{0.f, 1.f*(titleSize + 2*titlePadding) - titlePadding}
        ).Divide(2.f).Add(
            raylib::Vector2{0.f, 1.f*(titleSize + 2*titlePadding)}
        )};

    titlePos = raylib::Vector2{1.f*win->GetWidth()/2 - 1.f*raylib::MeasureText(title, titleSize)/2, titlePadding };

    auto start = make_shared<Button>("Start", butMidPoint, maxButtonWidth),
        options = make_shared<Button>("Options", butMidPoint, maxButtonWidth),
        credits = make_shared<Button>("Credits", butMidPoint, maxButtonWidth),
        exit = make_shared<Button>("Exit", butMidPoint, maxButtonWidth);
    _configButtons(start, options, credits, exit);

    start->setClick([&, winSize, midSize] () {
        drawStatic->clear();

        SpriteSheet boss{raylib::Image{"../resources/spritesheets/boss.png"}, 4, HORIZONTAL},
            bubble{raylib::Image{"../resources/spritesheets/bubble.png"}, 30, HORIZONTAL},
            bg{raylib::Image{"../resources/spritesheets/background.png"}, 1, HORIZONTAL};
        auto bgScale = max(1.f*winSize.x/bg.getFrameWidth(), 1.f*winSize.y/bg.getFrameHeight());

        shared_ptr<CutScene> _cut{make_shared<CutScene>(win, drawStack, drawStatic, *curScene)};
        _cut->appendSprite(bg, raylib::Vector2 {0, 0}, bgScale, true);

        boss.scale(0.7); bubble.scale(0.5);
        shared_ptr<Animation> bossAnimation = make_shared<Animation>(
            boss,
            12U,
            raylib::Vector2{midSize.x - boss.getFrameWidth() - 10, midSize.y - boss.getFrameHeight()/2}
        ),
            bubbleAnimation = make_shared<Animation>(
                bubble,
                12U,
                raylib::Vector2{midSize.x - 50, midSize.y - boss.getFrameHeight()/2}
            );
        _cut->appendSprite(bossAnimation, true);
        _cut->appendSprite(bubbleAnimation, false);

        _cut->setFrameRate(12U);

        shared_ptr<Game> temp{make_shared<Game>(win, drawStack, drawStatic, *curScene)};
        _cut->setScene(temp);

        shared_ptr<Scene> sCut{_cut};
        sCut.swap(*curScene);
        (*curScene)->init();
    });

    options->setClick([&] () {
        drawStatic->clear();
        shared_ptr<Scene> temp{make_shared<Options>(win, drawStack, drawStatic, *curScene)};
        temp.swap(*curScene);
        (*curScene)->init();
    });

    credits->setClick([&] () {
        drawStatic->clear();
        shared_ptr<Scene> temp = make_shared<Credits>(win, drawStack, drawStatic, *curScene);
        temp.swap(*curScene);
        (*curScene)->init();
    });

    exit->setClick([&]() {
        *curScene = nullptr;
    });

    for (auto &el : Logic::setPosStack(padding, VERTICAL, start, options, credits, exit))
        drawStatic->push_back(el);
}

void Menu::draw() {
    raylib::DrawText(title, titlePos.x, titlePos.y, titleSize, raylib::Color::Green());
}

void Menu::clearElements() {}

Menu::~Menu() {
}
