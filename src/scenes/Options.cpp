#include "./Options.hpp"
#include "./Menu.hpp"

#include "../include/json.hpp"

using std::make_pair;
using std::make_shared;
using std::max;
using std::ofstream;
using std::ifstream;

/**
 * @brief Construct a new Options:: Options object
 * 
 * @param _win Window of the corresponding scene.
 * @param _drawStack Vector that draws the stack elements. 
 * @param _drawStatic Vector that draws the stack elements.
 * @param _curScene Pointer to the current scene. 
 */
Options::Options(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
}

/**
 * @brief Saves current game options to a file.
 * 
 * @param path Path to the file containing the options.
 */
void Options::saveOptions(string path) {
    nlohmann::json options;
    options["difficulty"] = speed->getValue();

    ofstream save; save.open(path, std::ios::trunc);
    save << options.dump();
    save.close();

    std::cout << "Options saved to " << path << std::endl;
}

/**
 * @brief Loads the game options from a file.
 * 
 * @param path Path to the file containing the options.
 */
void Options::loadOptions(string path) {
    ifstream loaded; loaded.open(path);
    nlohmann::json obj;
    if (loaded.good()) {
        string raw;
        try {
            loaded >> raw;
            obj = nlohmann::json::parse(raw);
        } catch (...) {
            std::cerr << "Error while processing file" << std::endl;
        }
    } else {
        loaded.close();
        std::cerr << "File " << path << " could not be opened" << std::endl;
        return;
    }

    string conf{"difficulty"};
    if (obj.find(conf) != obj.end())
        speed->setValue(obj.at(conf).get<unsigned int>());

    loaded.close();
}

/**
 * @brief Changes the sprite of the Options scene
 * 
 * @param _sprite New sprite of the scene.
 */
void Options::setSprite(shared_ptr<Animation>& _sprite) {
    setSprite(_sprite, true);
}

/**
 * @brief Sets the sprite of the Options scene.
 * 
 * @param _sprite New sprite of the scene.
 * @param shouldLoop If the Animation of the sprite should loop.
 */
void Options::setSprite(shared_ptr<Animation>& _sprite, const bool shouldLoop) {
    sprite = make_pair(_sprite, shouldLoop);
}

/**
 * @brief Initializes all components of the Options scene
 * and puts them on the drawStack and drawStatic vector if
 * necessary.
 * 
 */
void Options::init() {
    constexpr int maxButtonWidth = 200;

    SpriteSheet bg{raylib::Image{"../resources/spritesheets/background.png"}, 1, HORIZONTAL};
    auto bgScale = max(1.f*win->GetWidth()/bg.getFrameWidth(), 1.f*win->GetHeight()/bg.getFrameHeight());
    bg.scale(bgScale);
    sprite = { make_shared<Animation>(bg, 12U, raylib::Vector2{0, 0}), false };
    sprite.first->startLoop(sprite.second);

    raylib::Vector2 winSize = win->GetSize(),
        midSize = winSize.Divide(2.f);

    speed = make_shared<Slider>(
        "Difficulty", 3U,
        midSize,
        maxButtonWidth
    );
    speed->setBorder(raylib::Color::DarkGreen());
    speed->setFontColor(raylib::Color::DarkGreen());

    shared_ptr<Button> saveBut{make_shared<Button>("Save", midSize)};
    saveBut->setClick([&]() {
        saveOptions();
    });

    uiEls = Logic::setPosStack(10, VERTICAL, speed, saveBut);

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

    for (auto i{uiEls.begin() + 1}; i != uiEls.end(); ++i)
        drawStatic->push_back(*i);

    loadOptions();
};

/**
 * @brief Draws everything that will be looping and 
 * possibly changing its state throughout the scene.
 * 
 * 
 */
void Options::draw() {
    auto key{GetKeyPressed()};
    if (speed->getState() && key) {
        switch (key) {
            case KEY_RIGHT:
                speed->increase();
                break;
            case KEY_LEFT:
                speed->decrease();
                break;
            case KEY_ENTER:
                speed->interact();
            default:
                break;
        }
    }
    drawStack->push_back(speed);
}

/**
 * @brief Destroy the Options:: Options object
 * 
 */
Options::~Options() {}
