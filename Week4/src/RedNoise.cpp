#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include "projection.h"

#define WIDTH 720
#define HEIGHT 480

void draw(DrawingWindow &window, glm::vec3 cam) {
	window.clearPixels();

	std::string obj_path = "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/Weekly Workbooks/04 Wireframes and Rasterising/models/cornell-box.obj";
	std::string mtl_path = "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/Weekly Workbooks/04 Wireframes and Rasterising/models/cornell-box.mtl";
	
	draw_projected_model(window, obj_path, mtl_path, WIDTH, HEIGHT, cam);
};

void update(DrawingWindow &window) {
};

void handleEvent(SDL_Event event, DrawingWindow &window, glm::vec3 &cam) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT){
			cam[0] = cam[0] - 0.05;
			draw(window, cam);
		}
		else if (event.key.keysym.sym == SDLK_RIGHT){
			cam[0] = cam[0] + 0.05;
			draw(window, cam);
		}
		else if (event.key.keysym.sym == SDLK_UP){
			cam[1] = cam[1] - 0.05;
			draw(window, cam);
		}
		else if (event.key.keysym.sym == SDLK_DOWN){
			cam[1] = cam[1] + 0.05;
			draw(window, cam);
		}
		else if (event.key.keysym.sym == SDLK_u){
			std::cout << "U" << std::endl;
			update(window);
		};
	} else if (event.type == SDL_MOUSEBUTTONDOWN) window.savePPM("output.ppm");
};

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	glm::vec3 cam(1, 0, 8);
	SDL_Event event;
	draw(window, cam);
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window, cam);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
};
