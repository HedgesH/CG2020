#include "projection.h"

const float PROJ_SCALE_FACTOR = 200;

glm::vec3 CAMERA;

float project(float f, float pos, int camera_index, float z, float size){
    float s1 = f + CAMERA[2];
    float s2 = (pos - CAMERA[camera_index]);
    float s3 = z - CAMERA[2];
    float s4 = int(round((PROJ_SCALE_FACTOR*s1*s2)/s3));
    int s5 = s4 + (size/2);
    // std::cout << " "<< std::endl;
    // std::cout << s1 << " " << f+4 << std::endl;
    // std::cout << s2 << " " << pos << std::endl;
    // std::cout << s3 << " " << z-4 << std::endl;
    // std::cout << s4 << " " << int(round((((PROJ_SCALE_FACTOR * (f+4) * pos)/(z-4))))) << std::endl;
    // std::cout << s5 << " " << CAMERA[0] << std::endl;
    return s5;

    // return int(round((((PROJ_SCALE_FACTOR * (f+4) * pos)/(z-4))) + (size/2)));
}

float hypotenuse(glm::vec3 v1, glm::vec3 v2){
    float dx = v1[0] - v2[0];
    float dy = v1[1] - v2[1];
    float dz = v1[2] - v2[2];
    return sqrt((dx*dx) + (dy*dy) + (dz*dz));
};

CanvasPoint project_vertex(glm::vec3 v, float width, float height){
    float xi = project(2, v[0] * -1, 0, v[2], width);
    float yi = project(2, v[1], 1, v[2], height);
    // std::cout << " " << std::endl;
    CanvasPoint v_image(xi, yi, v[2]);
    return v_image;
};

CanvasTriangle project_model_triangle(ModelTriangle model_triangle, float width, float height){
    CanvasTriangle projected_triangle;
    CanvasPoint v1 = project_vertex(model_triangle.vertices[0], width, height);
    CanvasPoint v2 = project_vertex(model_triangle.vertices[1], width, height);
    CanvasPoint v3 = project_vertex(model_triangle.vertices[2], width, height);
    projected_triangle = CanvasTriangle(v1, v2, v3);
    return projected_triangle;
}

std::vector<CanvasTriangle> get_projected_triangles(std::vector<ModelTriangle> model_triangles, float width, float height){
    std::vector<CanvasTriangle> projected_triangles;
    for(ModelTriangle model_triangle: model_triangles){projected_triangles.push_back(project_model_triangle(model_triangle, width, height));}
    return projected_triangles;
}

void draw_projected_model(DrawingWindow &window, std::string obj_path, std::string mtl_path, float width, float height, glm::vec3 cam){
    CAMERA = cam;
    std::vector<ModelTriangle> model_triangles = load_obj(obj_path, mtl_path);
    std::vector<CanvasTriangle> projected_triangles = get_projected_triangles(model_triangles, width, height);
    std::vector<CanvasPoint> window_points(width*height);
    std::vector<uint32_t> window_colours(width*height);

    for (size_t i = 0; i < width*height; i++)
    {
        window_colours[i] = 0;
        window_points[i] = CanvasPoint(0,0,-1*INFINITY);
    }
    
    for ( ModelTriangle v: model_triangles)
    {
        CanvasTriangle projected_triangle = project_model_triangle(v, width, height);
        std::vector<CanvasPoint> points = draw_tri_solid(projected_triangle);
        uint32_t colour = (255 << 24) + (v.colour.red << 16) + (v.colour.green << 8) + v.colour.blue;
        
        for (size_t i = 0; i < points.size(); i++)
        {
            CanvasPoint p = points[i];
            if((p.y*width) + p.x >= width * height || (p.y*width) + p.x < 0 ){
                continue;
            }
            float current_depth = window_points[(p.y*width) + p.x].depth;
            if(current_depth <= p.depth){
                window_points[(p.y*width) + p.x] = p;
                window_colours[(p.y*width) + p.x] = colour;
            }
        }
    }

    for (size_t i = 0; i < width*height; i++)
    {
        window.setPixelColour(window_points[i].x, window_points[i].y, window_colours[i]);
    }
    
};