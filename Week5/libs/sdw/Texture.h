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

std::vector<CanvasPoint> draw_tri_solid(CanvasTriangle tri);

std::vector<CanvasPoint> line(CanvasPoint start, CanvasPoint end);