#include <iostream>
#include <vector>
#include <glm/glm.hpp> 
#include "CanvasPoint.h"
#include "DrawingWindow.h"
#include "TextureMap.h"
#include "CanvasTriangle.h"
#include "ModelTriangle.h"
#include "TexturePoint.h"
#include <cmath>
#include "Utils.h"
#include "Colour.h"
#include <map>
#include "parse.h"
#include "Texture.h"

void draw_projected_model(DrawingWindow &window, std::string obj_path, std::string mtl_path, float width, float height, glm::vec3 cam);