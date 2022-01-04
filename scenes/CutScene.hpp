#ifndef CUTSCENE_H_
#define CUTSCENE_H_

#include "./Scene.cpp"

#include <vector>
#include <memory>

#include "../components/UIElement.cpp"
#include "../components/SpriteSheet.cpp"
#include "../components/Animation.cpp"
#include "../raylib-cpp/include/raylib-cpp.hpp"

class CutScene : public Scene {
	private:
		shared_ptr<vector<shared_ptr<UIElement>>> drawStack,
						  drawStatic;
		shared_ptr<raylib::Window> win;
		vector<pair<shared_ptr<Animation>, bool>> sprites{};
        unsigned int frameRate{1};
		bool changeState{false};
	public:
		CutScene() {}
		CutScene(shared_ptr<raylib::Window>& _win,
		 	shared_ptr<vector<shared_ptr<UIElement>>>& _drawStack,
			shared_ptr<vector<shared_ptr<UIElement>>>& _drawStatic
			);
        void appendSprite(shared_ptr<Animation>& newSprite, const bool shouldLoop);
        void appendSprite(SpriteSheet& spriteSheet,  
                        raylib::Vector2 startPos,                    
                        const bool shouldLoop
                    );
        void appendSprite(SpriteSheet& spriteSheet,  
                        raylib::Vector2 startPos,                    
                        float scale,      
                        const bool shouldLoop
                    );
        void setFrameRate(unsigned int _frameRate);
		void init();
		void draw();
};

#endif