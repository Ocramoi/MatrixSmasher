#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet(raylib::Image _sheet, unsigned int _amntFrames, Alignment _align) {
    sheet = _sheet;
    amntFrames = _amntFrames;
    align = _align;
}


raylib::Image SpriteSheet::getSheet() {
    return sheet;
}

void SpriteSheet::setSheet(raylib::Image _sheet) {
    sheet = _sheet;
}

unsigned int SpriteSheet::getAmntFrames() {
    return amntFrames;
}

void SpriteSheet::setAmntFrames(unsigned int _amntFrames) {
    amntFrames = _amntFrames;
}

Alignment SpriteSheet::getAlignment() {
    return align;
}

void SpriteSheet::setAlignment(Alignment _align) {
    align = _align;
}


