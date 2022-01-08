#include "GameOver.hpp"

/**
 * @brief Construct a new Game Over:: Game Over object
 * 
 * @param _win Window of the corresponding scene.
 * @param _drawStack Vector that draws the stack elements. 
 * @param _drawStatic Vector that draws the stack elements.
 * @param _curScene Pointer to the current scene. 
 */
GameOver::GameOver(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
}

/**
 * @brief Construct a new Game Over:: Game Over object
 * 
 * @param _win Window of the corresponding scene.
 * @param _drawStack Vector that draws the stack elements. 
 * @param _drawStatic Vector that draws the stack elements.
 * @param _curScene Pointer to the current scene.
 * @param _score Game score.
 */
GameOver::GameOver(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene,
    unsigned int _score
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
    score = _score;
    texts.back().first += std::to_string(score);
}

/**
 * @brief Sets the final score to be displayed on the
 * game over scene.
 * 
 * @param _score New score to be set.
 */
void GameOver::setFinalScore(unsigned int _score) {
    score = _score;
    texts.back().first = "Final score: " + std::to_string(score);
    setUI();
}

/**
 * @brief Configures all UI elements on the 
 * GameOver screen.
 * 
 */
void GameOver::setUI() {
    raylib::Vector2 winSize = win->GetSize(),
        midSize = winSize.Divide(2.f);

    txtPos.clear();
    txtPos.reserve(texts.size());

    auto totalHeight{texts.size()*textPadding + butCredits->getHeight()};
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

    butCredits->setPosition(raylib::Vector2{
        midSize.x - butCredits->getWidth()/2,
        tempH
    });
}

/**
 * @brief Initializes all components of the GameOver scene
 * and puts them on the drawStack and drawStatic vector if
 * necessary.
 */
void GameOver::init() {
    butCredits = make_shared<Button>("Scoreboard", raylib::Vector2{0, 0});
    butCredits->setBorder(raylib::Color::DarkGreen());
    butCredits->setFontColor(raylib::Color::DarkGreen());
    butCredits->setClick([&] () {
        drawStatic->clear();
        shared_ptr<Scene> temp = make_shared<Scoreboard>(win, drawStack, drawStatic, *curScene);
        temp.swap(*curScene);
        (*curScene)->init();
    });
    drawStatic->push_back(butCredits);

    auto backMenu = make_shared<Button>("Back to the menu", raylib::Vector2 {10, 10});
    backMenu->setBorder(raylib::Color::DarkGreen());
    backMenu->setFontColor(raylib::Color::DarkGreen());
    backMenu->setClick([&] () {
        drawStatic->clear();
        shared_ptr<Scene> temp = make_shared<Menu>(win, drawStack, drawStatic, *curScene);
        temp.swap(*curScene);
        (*curScene)->init();
    });
    drawStatic->push_back(backMenu);


    raylib::Vector2 winSize = win->GetSize();
    SpriteSheet boss{raylib::Image{"../resources/spritesheets/boss.png"}, 4, HORIZONTAL};

    boss.scale(0.3);
    constexpr float bossPadding{40.f};
    shared_ptr<Animation>
        bossNW = make_shared<Animation>(
            boss,
            12U,
            raylib::Vector2{bossPadding, bossPadding}
        ), bossNE = make_shared<Animation>(
            boss,
            12U,
            raylib::Vector2{winSize.x - bossPadding - boss.getFrameWidth(), bossPadding}
        ), bossSW = make_shared<Animation>(
            boss,
            12U,
            raylib::Vector2{bossPadding, winSize.y - bossPadding - boss.getFrameHeight()}
        ), bossSE = make_shared<Animation>(
            boss,
            12U,
            raylib::Vector2{winSize.x - bossPadding - boss.getFrameWidth(), winSize.y - bossPadding - boss.getFrameHeight()}
        );
    drawStatic->push_back(bossNW); drawStatic->push_back(bossNE); drawStatic->push_back(bossSW); drawStatic->push_back(bossSE);
    bossNW->startLoop(); bossNE->startLoop(); bossSW->startLoop(); bossSE->startLoop();

    setUI();
}

/**
 * @brief Draws everything that will be looping and 
 * possibly changing its state throughout the scene
 */
void GameOver::draw() {
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
 * @brief Destroy the Game Over:: Game Over object
 * 
 */
GameOver::~GameOver() {
    drawStack->clear();
}
