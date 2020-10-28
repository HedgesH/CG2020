#include <iostream>
#include <vector>
#include <glm/vec3.hpp> 
#include "CanvasPoint.h"
#include "DrawingWindow.h"
#include "TextureMap.h"
#include "CanvasTriangle.h"
#include "TexturePoint.h"
#include <cmath>

void draw_tri_texture(DrawingWindow &window, CanvasTriangle tri, std::string texture_path);

void draw_tri_solid(DrawingWindow &window, CanvasTriangle tri);