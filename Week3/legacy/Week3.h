#include <iostream>
#include <vector>
#include <glm/vec3.hpp> 
#include <glm/vec2.hpp> 
#include "CanvasPoint.h"
#include "DrawingWindow.h"
#include "TextureMap.h"
#include <cmath>

void draw_line(DrawingWindow &window, CanvasPoint start, CanvasPoint end, glm::vec3 color_vector);
void draw_unfilled_triangle(DrawingWindow &window, CanvasPoint p1, CanvasPoint p2, CanvasPoint p3, glm::vec3 color_vector);
void draw_random_unfilled_triangle(DrawingWindow &window);
void draw_random_filled_triangle(DrawingWindow &window);
void draw_texture_triangle(DrawingWindow &window);
void draw_texture_triangle(DrawingWindow &window, std::vector<CanvasPoint> triangle_pos);