#include "./Game.hpp"
#include "./Credits.hpp"

#include "../include/json.hpp"

using std::max;
using std::random_device;
using std::ifstream;
using std::ofstream;

/**
 * @brief Construct a new Game:: Game object.
 * 
 * @param _win Window of the corresponding scene.
 * @param _drawStack Vector that draws the stack elements. 
 * @param _drawStatic Vector that draws the stack elements.
 * @param _curScene Pointer to the current scene.
 */
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


/**
 * @brief Load the options file containing all previously 
 * saves game configurations.
 * 
 * @param path Path to the options.
 */
void Game::loadOptions(string path) {
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
		diff = obj.at(conf).get<unsigned int>();

	wordSpeed *= 1 + 0.1f*diff;

    loaded.close();
}

/**
 * @brief Saves/updates the current grame score to a bin
 * file.
 * 
 * @param path Path to the save file.
 */
void Game::saveScore(string path) {
	vector<pair<score_t, difficulty_t>> scores{{ points, diff }};

	ifstream sRaw; sRaw.open(path, std::ios::in | std::ios::binary);
	if (sRaw.good()) {
		sRaw.seekg(0, std::ios::end);
		auto nScores{sRaw.tellg()/(sizeof(score_t) + sizeof(difficulty_t))};
		sRaw.seekg(0, std::ios::beg);
		scores.reserve(nScores + 1);
		for (decltype(nScores) i{0}; i < nScores; ++i) {
			score_t curS; sRaw.read(reinterpret_cast<char*>(&curS), sizeof(score_t));
			difficulty_t curD; sRaw.read(reinterpret_cast<char*>(&curD), sizeof(difficulty_t));
			scores.push_back({ curS, curD });
		}
		std::cout << "Info file read" << std::endl;
	} else
		std::cerr << "IN: Error while opening info file" << std::endl;
	sRaw.close();

	std::sort(scores.begin(), scores.end());
	ofstream oScores; oScores.open(path, std::ios::trunc | std::ios::binary);
	if (oScores.good()) {
		auto it{scores.rbegin()};
		for (unsigned int i{0}; i < SCOREBOARD_SIZE && it != scores.rend(); ++it, ++i) {
			oScores.write(reinterpret_cast<char*>(&it->first), sizeof(it->first));
			oScores.write(reinterpret_cast<char*>(&it->second), sizeof(it->second));
		}
		std::cout << "Info file written" << std::endl;
	} else
		std::cerr << "OUT: Error while opening info file" << std::endl;
	oScores.close();
}

/**
 * @brief Loads the words used in the game.
 * 
 * @param path Path to the file containing the words.
 */
void Game::loadWords(string path) {
	ifstream wordStream; wordStream.open(path, ifstream::in);
	string tmp;
	while (wordStream >> tmp) availableWords.push_back(tmp);
}

/**
 * @brief Feed the words to the screen. Main logic of the game, 
 * based of the Producer-Consumer problem.
 * 
 * @param _game 
 */
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

		{
			unique_lock<mutex> uFeed(_game->feedMutex);
			_game->conditionalFeed.wait_for(
				uFeed,
				std::chrono::milliseconds(static_cast<int>(timeout))
			);
		}
	}
}

/**
 * @brief Initializes all components of the Game.
 * 
 */
void Game::init() {
	loadOptions();
	feeder = thread([&]() { Game::_feed(this); });
}

/**
 * @brief Draws everything that will be looping and 
 * possibly changing its state throughout the scene
 */
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

	if (keyPress && !anyHits) points *= 0.95;

	if (end) endGame();
}

/**
 * @brief Sets the initial game speed 
 * 
 * @param _speed New speed to be set.
 */
void Game::setSpeed(unsigned int _speed) {
	speed = _speed;
}

/**
 * @brief Saves and changes scenes once the 
 * game over condition is reached. 
 * 
 */
void Game::endGame() {
	shared_ptr<Scene> temp = make_shared<GameOver>(win, drawStack, drawStatic, *curScene, points);
	saveScore();
	drawStatic->clear();
	stopFeed.store(true);
	conditionalFeed.notify_all();
	feeder.join();
	temp->init();
	temp.swap(*curScene);
}

Game::~Game() {}
