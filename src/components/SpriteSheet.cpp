#include "SpriteSheet.hpp"

/**
 * @brief Construct a new Sprite Sheet:: Sprite Sheet object.
 * 
 * @param _sheet Corresponding image of the sprite sheet.
 * @param _amntFrames Amount of frames in the sprite sheet.
 * @param _align Sprite sheet's alignment.
 */
SpriteSheet::SpriteSheet(const raylib::Image& _sheet, unsigned int _amntFrames, Alignment _align) {
    setSheet(_sheet);
    amntFrames = _amntFrames;
    align = _align;
}

/**
 * @brief Returns the scaled sprite sheet image.
 * 
 * @return `raylib::Image` scaled sprite sheet. 
 */
raylib::Image SpriteSheet::getSheet() {
    return *scaled;
}

/**
 * @brief Returns the texture loaded from the sprite sheet itself.
 * 
 * @return `Texture2D&` loaded sprite sheet texture. 
 */
Texture2D& SpriteSheet::getTexture() {
    return texture;
}

/**
 * @brief Changes the spritesheet image.
 * 
 * @param _sheet New image of the sprite sheet. 
 */
void SpriteSheet::setSheet(const raylib::Image& _sheet) {
    sheet = std::make_shared<raylib::Image>(_sheet);
    scaled = std::make_shared<raylib::Image>(_sheet);
    texture = scaled->LoadTexture();
}

/**
 * @brief Returns the amount of frames of the sprite sheet.
 * 
 * @return `unsigned int` amount of frames.
 */
unsigned int SpriteSheet::getAmntFrames() {
    return amntFrames;
}

/**
 * @brief Changes the amount of frames of a spritesheet.
 * 
 * @param _amntFrames New amount of frames.s
 */
void SpriteSheet::setAmntFrames(unsigned int _amntFrames) {
    amntFrames = _amntFrames;
}

/**
 * @brief Gets the corresponding alignment of the sprite.
 * 
 * @return Alignment HORIZONTAL or VERTICAL.
 */
Alignment SpriteSheet::getAlignment() {
    return align;
}

/**
 * @brief Changes the alignment of the sprite.
 * 
 * @param _align Can either be VERTICAL or HORIONTAL.
 */
void SpriteSheet::setAlignment(Alignment _align) {
    align = _align;
}
 
/**
 * @brief Gets the width of each frame of the sprite sheet
 * 
 * @return `float` frames width 
 */
float SpriteSheet::getFrameWidth() {
    return (align == HORIZONTAL) ? (scaled->GetWidth() / amntFrames) : scaled->GetWidth();
}

/**
 * @brief Gets the height of each frame of the sprite sheet
 * 
 * @return `float` frames height 
 */
float SpriteSheet::getFrameHeight() {
    return (align == VERTICAL) ? (scaled->GetHeight() / amntFrames) : scaled->GetHeight();
}

/**
 * @brief Scales the sprite sheet to its new size
 * 
 * @param newScale Percentage of the new scale.
 */
void SpriteSheet::scale(float newScale) {
    scaled = std::make_shared<raylib::Image>(sheet->Resize(
        newScale*sheet->GetWidth(),
        newScale*sheet->GetHeight()
    ));
    texture = scaled->LoadTexture();
}

SpriteSheet::~SpriteSheet() {
    // UnloadTexture(texture);
}
