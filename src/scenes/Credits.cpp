#include "./Credits.hpp"
#include "./Menu.hpp"

#include <algorithm>

using std::make_pair;
using std::make_shared;
using std::for_each;
using std::max;

/**
 * @brief Construct a new Credits:: Credits object
 * 
 * @param _win Window of the corresponding scene.
 * @param _drawStack Vector that draws the stack elements. 
 * @param _drawStatic Vector that draws the stack elements.
 * @param _curScene Pointer to the current scene.
 */
Credits::Credits(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
}

/**
 * @brief Construct a new Credits:: Credits object
 * 
 * @param _win Window of the corresponding scene.
 * @param _drawStack Vector that draws the stack elements. 
 * @param _drawStatic Vector that draws the stack elements.
 * @param _curScene Pointer to the current scene. 
 * @param extraText Extra text to be draw on screen.
 */
Credits::Credits(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene,
    vector<pair<string, unsigned int>> extraText
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
    for (const auto& e : extraText) texts.push_back(e);
}

/**
 * @brief Changes the sprite of the Credits scene
 * 
 * @param _sprite New sprite of the scene.
 */
void Credits::setSprite(shared_ptr<Animation>& _sprite) {
    setSprite(_sprite, true);
}

/**
 * @brief Sets the sprite of the Credits scene.
 * 
 * @param _sprite New sprite of the scene.
 * @param shouldLoop If the Animation of the sprite should loop.
 */
void Credits::setSprite(shared_ptr<Animation>& _sprite, const bool shouldLoop) {
    sprite = make_pair(_sprite, shouldLoop);
}

/**
 * @brief Initializes all components of the Credits scene
 * and puts them on the drawStack and drawStatic vector if
 * necessary.
 * 
 */
void Credits::init() {
    SpriteSheet bg{raylib::Image{"../resources/spritesheets/background.png"}, 1, HORIZONTAL};
    auto bgScale = max(1.f*win->GetWidth()/bg.getFrameWidth(), 1.f*win->GetHeight()/bg.getFrameHeight());
    bg.scale(bgScale);
    sprite = { make_shared<Animation>(bg, 12U, raylib::Vector2{0, 0}), false };
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

    raylib::Vector2 winSize = win->GetSize(),
        midSize = winSize.Divide(2.f);
    txtPos.reserve(texts.size());

    int totalHeight{max((static_cast<int>(texts.size()) - 1)*textPadding, 0)};
    for_each(
        texts.begin(),
        texts.end(),
        [&totalHeight] (decltype(texts.front()) t) { totalHeight += t.second; }
    );
    auto tempH{midSize.y - totalHeight/2};
    for (const auto &t : texts) {
        txtPos.push_back(raylib::Vector2{
            1.f*midSize.x - 1.f*raylib::MeasureText(t.first, t.second)/2,
            tempH
        });
        tempH += t.second + textPadding;
    }
};

/**
 * @brief Draws everything that will be looping and 
 * possibly changing its state throughout the scene
 * 
 */
void Credits::draw() {
    for (decltype(texts.size()) i{0}; i < texts.size(); ++i)
        raylib::DrawText(
            texts.at(i).first,
            txtPos.at(i).x,
            txtPos.at(i).y,
            texts.at(i).second,
            txtColor
        );
}

/**
 * @brief Destroy the Credits:: Credits object
 * 
 */
Credits::~Credits() {
    sprite.first.reset();
}
