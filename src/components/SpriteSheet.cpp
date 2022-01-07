#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet(const raylib::Image& _sheet, unsigned int _amntFrames, Alignment _align) {
    setSheet(_sheet);
    amntFrames = _amntFrames;
    align = _align;
}

raylib::Image SpriteSheet::getSheet() {
    return *scaled;
}

Texture2D& SpriteSheet::getTexture() {
    return texture;
}

void SpriteSheet::setSheet(const raylib::Image& _sheet) {
    sheet = std::make_shared<raylib::Image>(_sheet);
    scaled = std::make_shared<raylib::Image>(_sheet);
    texture = scaled->LoadTexture();
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
 
float SpriteSheet::getFrameWidth() {
    return (align == HORIZONTAL) ? (scaled->GetWidth() / amntFrames) : scaled->GetWidth();
}

float SpriteSheet::getFrameHeight() {
    return (align == VERTICAL) ? (scaled->GetHeight() / amntFrames) : scaled->GetHeight();
}

void SpriteSheet::scale(float newScale) {
    scaled = std::make_shared<raylib::Image>(sheet->Resize(
        newScale*sheet->GetWidth(),
        newScale*sheet->GetHeight()
    ));
    texture = scaled->LoadTexture();
}
