#ifndef SPRITESHEET_H_
#define SPRITESHEET_H_

#include <memory>
#include <iostream>
#include "../raylib-cpp/include/raylib-cpp.hpp"

using std::string;

enum _alignment {
    HORIZONTAL,
    VERTICAL
};

using Alignment = _alignment;

class SpriteSheet {
    private:
        // float scale{1.f};
        std::shared_ptr<raylib::Image> sheet, scaled;
        // raylib::Image sheet{""}, scaled{""};
        Texture2D texture;
        unsigned int amntFrames{1};
        Alignment align;

    public:
        SpriteSheet() {}
        ~SpriteSheet() {
            UnloadTexture(texture);
        }
        SpriteSheet(const raylib::Image& _sheet, unsigned int _amntFrames, Alignment _align);
        SpriteSheet(const raylib::Image& _sheet, unsigned int _amntFrames, float _scale, Alignment _align);
        raylib::Image getSheet();
        Texture2D& getTexture();
        void setSheet(const raylib::Image& _sheet);
        unsigned int getAmntFrames();
        void setAmntFrames(unsigned int _amntFrames);
        Alignment getAlignment();
        void setAlignment(Alignment _align);
        float getFrameWidth();
        float getFrameHeight();
        void scale(float newScale);
};

#endif