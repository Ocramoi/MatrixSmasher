#include "./Game.hpp"

using std::max;

void Game::_feed(Game* _game) {
	while (true) {
		{
			lock_guard<mutex> guard{_game->wlMutex};
			_game->words.emplace("aaa", *_game->win);
		}

		auto salt = (1.f*rand()/RAND_MAX)*_game->var - _game->var/2.f, // TODO !!
			timeout = _game->speed + salt;
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
	feeder = thread([&]() { Game::_feed(this); });
	feeder.detach();
}

void Game::init() {}

void Game::draw() {
	auto keyPress{GetCharPressed()};

	decltype(words.size()) qnt;
	{
		lock_guard<mutex> guard{wlMutex};
		qnt = words.size();
	}

	while (qnt-- > 0) {
		auto temp = words.front(); words.pop();
		temp.draw();
		temp.fall();
		if (!temp.input(keyPress) && temp.getPos().y < win->GetHeight()) words.push(temp);
		else --lives;

		if (lives < 0) endGame();
	}
}

void Game::setSpeed(unsigned int _speed) {
	speed = _speed;
}

void Game::endGame() {
}