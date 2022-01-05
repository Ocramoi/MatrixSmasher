#ifndef LOGIC_H_
#define LOGIC_H_

#include <optional>
#include <iostream>
#include <string_view>
#include <memory>
#include <vector>
#include "../raylib-cpp/include/raylib-cpp.hpp"
#include "./UIElement.cpp"

using std::string_view;
using std::optional;
using std::string;
using std::pair;
using std::shared_ptr;
using std::vector;

namespace Logic {
    constexpr string_view PROJECT = "Storytelling",
        RESOURCES_PATH = "resources/";
    constexpr int RESOURCE_OFFSET = 2;

    // void drawHeader(Game &game, const raylib::Window &win);
    // void drawFPS(Game &game, const raylib::Window &win);
    pair<string, unsigned short> clampText(
        const string &str,
        int maxWidth,
        int fontSize);
    pair<string, unsigned short> clampText(
        const string &str,
        int maxWidth,
        int fontSize,
        unsigned int& maxTok);
    void _addStack(vector<shared_ptr<UIElement>>& Q, const shared_ptr<UIElement>& el);

    template <class... UIs>
    void _addStack(vector<shared_ptr<UIElement>>& Q, const shared_ptr<UIElement>& el, UIs... els) {
        Q.push_back(el);
        _addStack(Q, els...);
    }
    template <class... UIs>
    vector<shared_ptr<UIElement>> setPosStack(int padding, Alignment align, const shared_ptr<UIElement>& el, UIs... els) {
        vector<shared_ptr<UIElement>> Q{};
        _addStack(Q, el, els...);
        
        int sumHeight{-padding};
        int sumWidth{-padding};
        for (const auto& e : Q) {
            sumHeight += e->getHeight() + padding;
            sumWidth += e->getWidth() + padding;
        }

        int tempHeight{sumHeight/2};
        int tempWidth{sumWidth/2};
        for (auto& e : Q) {
            if (align == VERTICAL) 
                e->setPosition(e->getPosition().Subtract({ e->getWidth()*1.f/2, tempHeight*1.f }));
            else 
                e->setPosition(e->getPosition().Subtract({tempWidth*1.f, e->getHeight()*1.f/2}));

            tempHeight -= e->getHeight() + padding;
            tempWidth -= e->getWidth() + padding;
        }

        return Q;
    }
}


#endif // LOGIC_H_
