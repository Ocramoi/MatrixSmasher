#include "Scoreboard.hpp"

using std::to_string;

Scoreboard::Scoreboard(
    shared_ptr<raylib::Window>& _win,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
    shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic,
    shared_ptr<Scene>& _curScene
) {
    win = _win; drawStack = _drawStack; drawStatic = _drawStatic;
    curScene = &_curScene;
}

Scoreboard::~Scoreboard() {}

void Scoreboard::loadScores(string path) {
    ifstream sRaw; sRaw.open(path, std::ios::in | std::ios::binary);
    if (sRaw.good()) {
        sRaw.seekg(0, std::ios::end);
        auto nScores{sRaw.tellg()/(sizeof(score_t) + sizeof(difficulty_t))};
        sRaw.seekg(0, std::ios::beg);
        texts.reserve(texts.size() + nScores);
        for (decltype(nScores) i{0}; i < nScores; ++i) {
            score_t curS; sRaw.read(reinterpret_cast<char*>(&curS), sizeof(score_t));
            difficulty_t curD; sRaw.read(reinterpret_cast<char*>(&curD), sizeof(difficulty_t));
            texts.push_back({ to_string(i + 1) + ": " + to_string(curS) + " - difficulty: " + to_string(curD), 25.f });
        }
    }
    sRaw.close();
}

void Scoreboard::init() {
    loadScores();

    raylib::Vector2 winSize = win->GetSize(),
        midSize = winSize.Divide(2.f);
    txtPos.reserve(texts.size());

    auto back = make_shared<Button>("Back", raylib::Vector2 {10, 10});
    back->setBorder(raylib::Color::DarkGreen());
    back->setFontColor(raylib::Color::DarkGreen());
    back->setClick([&] () {
        drawStatic->clear();
        shared_ptr<Scene> temp = make_shared<Menu>(win, drawStack, drawStatic, *curScene);
        temp.swap(*curScene);
        (*curScene)->init();
    });
    drawStatic->push_back(back);

    int totalHeight{std::max((static_cast<int>(texts.size()) - 1)*textPadding, 0)};
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
}

void Scoreboard::draw() {
    for (decltype(texts.size()) i{0}; i < texts.size(); ++i)
        raylib::DrawText(
            texts.at(i).first,
            txtPos.at(i).x,
            txtPos.at(i).y,
            texts.at(i).second,
            txtColor
        );
}
