#include "./Game.hpp"
#include "./Credits.hpp"

using std::max;
using std::random_device;

Game::Game(
	shared_ptr<raylib::Window>& _win,
	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
	shared_ptr<Scene>& _curScene
) {
	win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
	curScene = &_curScene;
	#ifdef DEBUG
	lives = 2;
	#endif
	random_device r; rE = default_random_engine(r());
	loadWords();
}

void Game::loadWords(string path) {
	ifstream wordStream; wordStream.open(path, ifstream::in);
	string tmp;
	while (wordStream >> tmp) availableWords.push_back(tmp);
}

void Game::_feed(Game* _game) {
	std::uniform_int_distribution<int> dist(0, _game->availableWords.size() - 1);
	std::uniform_real_distribution<float> saltDist(0.7f, 1.3f);
	while (true) {
		if (_game->stopFeed.load()) break;

		auto idx = dist(_game->rE);
		{
			lock_guard<mutex> guard{_game->wlMutex};
			_game->words.emplace(_game->availableWords.at(idx), *_game->win);
		}

		_game->speed *= 0.98;
		auto timeout = max(_game->speed * saltDist(_game->rE), 300.f);
		unique_lock<mutex> uFeed(_game->feedMutex);

		_game->conditionalFeed.wait_for(
			uFeed,
			std::chrono::milliseconds(static_cast<int>(timeout))
		);
	}
}

void Game::init() {
	feeder = thread([&]() { Game::_feed(this); });
}

void Game::draw() {
	decltype(words.size()) qnt;
	{
		lock_guard<mutex> guard{wlMutex};
		qnt = words.size();
	}
	unsigned int textPadding{10},
		tempH{textPadding};
	raylib::DrawText("Current lives: " + std::to_string(lives), 10, tempH, fontSize, raylib::Color::Green());
	tempH += fontSize + textPadding;
	raylib::DrawText("Score: " + std::to_string(points), 10, tempH, fontSize, raylib::Color::Green());
	tempH += fontSize + textPadding;
	raylib::DrawText("Words on screen: " + std::to_string(qnt), 10, tempH, fontSize, raylib::Color::Green());

	auto keyPress{GetCharPressed()};
	bool anyHits{false},
		end{false};
	while (qnt-- > 0) {
		auto temp = words.front(); words.pop();
		auto wordInteract{temp.input(keyPress)};

		anyHits |= wordInteract.first;

		if (wordInteract.second) {
			points += 100;
			continue;
		}

		temp.draw();
		temp.fall();
		if (temp.getPos().y < win->GetHeight() + 15) {
			lock_guard<mutex> guard(wlMutex);
			words.push(temp);
		}
		else --lives;

		if (lives <= 0) {
			end = true;
			break;
		}
	}

	if (keyPress && !anyHits) points *= 0.9;

	if (end) endGame();
}

void Game::setSpeed(unsigned int _speed) {
	speed = _speed;
}

void Game::endGame() {
	drawStatic->clear();
	stopFeed.store(true);
	conditionalFeed.notify_all();
	feeder.join();
	shared_ptr<Scene> temp = make_shared<GameOver>(win, drawStack, drawStatic, *curScene, points);
	temp->init();
	temp.swap(*curScene);
}

Game::~Game() {}
