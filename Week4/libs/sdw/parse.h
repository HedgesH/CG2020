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

const float SCALING_FACTOR = 0.17;

std::vector<ModelTriangle> load_obj(std::string obj_path, std::string mtl_path);