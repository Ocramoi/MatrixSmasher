#include "./Menu.hpp"

Menu::Menu(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
	) {
    win = _win; 
    drawStack = _drawStack; 
    drawStatic = _drawStatic;

    raylib::Vector2 winSize = win->GetSize();
    _configButtons(buttons, 100, winSize.Divide(2.f));
}

void Menu::_configButtons(vector<shared_ptr<UIElement>>& buttons, const int maxButtonWidth, raylib::Vector2 startPos) {
    auto start = make_shared<Button>("Start", startPos, maxButtonWidth), 
        options = make_shared<Button>("Options", startPos, maxButtonWidth),
        exit = make_shared<Button>("Exit", startPos, maxButtonWidth);

    start->setBorder(raylib::Color::DarkGreen());
    start->setFontColor(raylib::Color::DarkGreen());
    options->setBorder(raylib::Color::DarkGreen());
    options->setFontColor(raylib::Color::DarkGreen());
    exit->setBorder(raylib::Color::DarkGreen());
    exit->setFontColor(raylib::Color::DarkGreen());

    start->setClick([&] () {
        sceneState = CUTSCENE;
        startAnimation(win, drawStatic, sceneState);
    });

    options->setClick([&sceneState] () {
        sceneState = OPTIONS;
    });

    exit->setClick([&sceneState] () {
        sceneState = EXIT;
    });

    buttons.push_back(start);
    buttons.push_back(options);
    buttons.push_back(exit);
}

void Menu::init() {}

void Menu::draw() {
	for (auto& el : *drawStatic) el->draw();
	for (auto& el : *drawStack) el->draw();
	drawStack->clear();
}

vector<shared_ptr<UIElement>> Menu::getButtons() {
    return buttons;
}