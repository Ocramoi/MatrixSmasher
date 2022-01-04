#include "./Menu.hpp"


Menu::Menu(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
	) {
    constexpr int maxButtonWidth = 100;
    raylib::Vector2 winSize = win.GetSize(),
    midSize = winSize.Divide(2.f);

    auto start = make_shared<Button>("Start", midSize, maxButtonWidth), 
        options = make_shared<Button>("Options", midSize, maxButtonWidth),
        exit = make_shared<Button>("Exit", midSize, maxButtonWidth);

    start->setClick([&] () {
        sceneState = CUTSCENE;
        startAnimation(win, drawStatic, sceneState);
    });
    start->setBorder(raylib::Color::DarkGreen());
    start->setFontColor(raylib::Color::DarkGreen());

    options->setClick([&sceneState] () {
        sceneState = scene::OPTIONS;
    });
    options->setBorder(raylib::Color::DarkGreen());
    options->setFontColor(raylib::Color::DarkGreen());

    exit->setClick([&sceneState] () {
        sceneState = scene::EXIT;
    });
    
    exit->setBorder(raylib::Color::DarkGreen());
    exit->setFontColor(raylib::Color::DarkGreen());
   
}


void Menu::init() {
}

void Menu::draw() {
    win.BeginDrawing();

	for (auto& el : *drawStatic) el->draw();
	for (auto& el : *drawStack) el->draw();
	drawStack->clear();

	win.EndDrawing();
}