#ifndef SPRITESHEET_H_
#define SPRITESHEET_H_

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
        raylib::Image sheet{""};
        unsigned int amntFrames;
        Alignment align;
    
    public:
        SpriteSheet() {}
        SpriteSheet(raylib::Image _sheet, unsigned int _amntFrames, Alignment _align);
        raylib::Image getSheet();
        void setSheet(raylib::Image _sheet);
        unsigned int getAmntFrames();
        
        void setAmntFrames(unsigned int _amntFrames);
        Alignment getAlignment();
        void setAlignment(Alignment _align);

        raylib::Rectangle getFrameRec();
};

#endif