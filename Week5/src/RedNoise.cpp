#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <map>
#include "projection.h"

#define WIDTH 720
#define HEIGHT 480
#define SPEED_X 0.5
#define SPEED_Y 0.5
#define SPEED_Z 1.5

void draw(DrawingWindow &window, Projection &proj) {
	window.clearPixels();
	std::string test_path = "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/Weekly Workbooks/04 Wireframes and Rasterising/models/test3.obj";
	proj.draw_projected_model(window);
};

void update(DrawingWindow &window, Projection &proj) {
	proj.rotate_camera_y(true);
	proj.lookaty();
	draw(window, proj);
};

void handleEvent(SDL_Event event, DrawingWindow &window, Projection &proj) {
	glm::vec3 &cam = proj.camera;
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_a){
			proj.trans_camera(SPEED_X, 0, 0);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_d){
			proj.trans_camera(-1*SPEED_X, 0, 0);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_w){
			proj.trans_camera(0, SPEED_Y, 0);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_s){
			proj.trans_camera(0, -1*SPEED_Y, 0);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_q){
			proj.trans_camera(0, 0, SPEED_Z);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_e){
			proj.trans_camera(0, 0, -1*SPEED_Z);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_r){
			proj.rotate_camera_x(true);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_t){
			proj.rotate_camera_x(false);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_LEFT){
			proj.pan_camera_y(true);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_RIGHT){
			proj.pan_camera_y(false);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_UP){
			proj.tilt_camera_x(false);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_DOWN){
			proj.tilt_camera_x(true);
			draw(window, proj);
		}
		else if (event.key.keysym.sym == SDLK_u){
			proj.lookaty();
			draw(window, proj);
		};
	} else if (event.type == SDL_MOUSEBUTTONDOWN) window.savePPM("output.ppm");
};

int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	glm::vec3 cam(0, 0, 4);
	float angle = 0;
	SDL_Event event;
	std::string obj_path = "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/Week5/textured-cornell-box.obj";
	std::string mtl_path = "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/Week5/textured-cornell-box.mtl";
	std::vector<ObjFileObject> model_triangles = load_obj(obj_path, mtl_path);
	std::map<std::string, TextureMap> texture_dict;
	load_texture_dict(mtl_path, texture_dict);

	Projection proj(model_triangles, texture_dict, WIDTH, HEIGHT, cam, angle);
	draw(window, proj);
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window, proj);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		update(window, proj);
		window.renderFrame();
	}
};
