#include "./Game.hpp"

using std::max;

void Game::_feed(Game& _game) {
	while (true) {
		_game.words.load().emplace("aaa");

		auto salt = rand()*_game.var - _game.var/2.f, // TODO !!
			timeout = _game.speed + salt;
		timeout = max(timeout, 100.f);
		std::this_thread::sleep_for(
			std::chrono::milliseconds(static_cast<int>(timeout))
		);
	}
}

Game::Game(
	shared_ptr<raylib::Window>& _win, 
	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack, 
	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
) {
	win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
	feeder = thread([&]() { Game::_feed(*this); });
	feeder.detach();
}

void Game::init() {}

void Game::draw() {
	auto keyPress{GetCharPressed()};

	auto qnt{words.load().size()};
	while (qnt-- > 0) {
		auto temp = words.load().front(); words.load().pop();
		temp.fall();
		if (!temp.input(keyPress) && temp.getPos().y < win->GetHeight()) words.load().push(temp);
	}
}

void Game::setSpeed(unsigned int _speed) {
	speed = _speed;
}