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

// void draw_projected_model(DrawingWindow &window, std::vector<ObjFileObject> model_triangles, std::map<std::string,
//                           TextureMap> texture_map_dict, float width, float height, glm::vec3 cam, float angle);

// void rotate_camera(float angle, glm::vec3 &cam);

class Projection{

    public:
        std::map<std::string, TextureMap> texture_map_dict;
        std::vector<ObjFileObject> model_triangles;
        float width;
        float height;
        glm::vec3 camera;
        glm::mat3 camera_orientation;
        float angle;
        const float PROJ_SCALE_FACTOR = 500;
        const float FOCAL_LENGTH = 2;
        const float ANGLE = 1.50;
        glm::vec3 center;

        Projection(std::vector<ObjFileObject> model_triangles,
                   std::map<std::string, TextureMap> texture_map_dict, float width, float height, glm::vec3 cam, float angle);

        void draw_projected_model(DrawingWindow &window);
        void set_camera(glm::vec3 camera);
        void set_angle(float angle);
        void rotate_camera_y(bool dir);
        void rotate_camera_x(bool dir);
        void pan_camera_y(bool dir);
        void tilt_camera_x(bool dir);
        void trans_camera(float x, float y, float z);
        void lookaty();
        void lookatx();
    private:
        CanvasTriangle project_model_triangle(ModelTriangle model_triangle, bool texture);
        CanvasPoint project_vertex(glm::vec3 v);
        float hypotenuse(glm::vec3 v1, glm::vec3 v2);
        float project(float pos, float index, float z);
        void to_coord();



};