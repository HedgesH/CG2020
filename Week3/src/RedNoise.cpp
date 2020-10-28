#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include "Texture.h"

#define WIDTH 320
#define HEIGHT 240

void draw(DrawingWindow &window) {
	window.clearPixels();
	CanvasPoint p1(160,10);
	CanvasPoint p2(300,230);
	CanvasPoint p3(10,150);
	
	p1.texturePoint = TexturePoint(195, 5);
	p2.texturePoint = TexturePoint(395, 380);
	p3.texturePoint = TexturePoint(65, 330);

	CanvasTriangle tri(p1, p2, p3);
	
	draw_tri_texture(window, tri, "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/resources/texture.ppm");
};

void update(DrawingWindow &window) {
	// Function for performing animation (shifting artifacts or moving the camera)
};

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
		else if (event.key.keysym.sym == SDLK_u){
			std::cout << "U" << std::endl;
			update(window);
		};
	} else if (event.type == SDL_MOUSEBUTTONDOWN) window.savePPM("output.ppm");
};

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	draw(window);
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
};
