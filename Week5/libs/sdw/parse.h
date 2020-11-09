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

struct ObjFileObject {
	std::string name;
	bool texture;
    std::string tpath;
    Colour colour;
    std::vector<ModelTriangle> tris;
    ObjFileObject();
	ObjFileObject(std::string n, Colour c);
    ObjFileObject(std::string n, Colour c, std::string tpath);
};

void load_texture_dict(std::string path, std::map<std::string, TextureMap> &texture_dict);

std::vector<ObjFileObject> load_obj(std::string obj_path, std::string mtl_path);