#include "projection.h"



void Projection::to_coord(){
    float counter = 0;
    glm::vec3 center(0, 0, 0);
    for(ObjFileObject o: model_triangles){
        for(ModelTriangle tri: o.tris){ 
            for(glm::vec3 p: tri.vertices){
                center = center + p;
                counter = counter + 1;
            }
        }
    }
    this->center = (center/counter);
};

void print_vec3(glm::vec3 v){
    std::cout << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]" << std::endl;
}

Projection::Projection(std::vector<ObjFileObject> model_triangles,
                   std::map<std::string, TextureMap> texture_map_dict, float width, float height, glm::vec3 cam, float angle){

    this->texture_map_dict = texture_map_dict;
    this->model_triangles = model_triangles;
    this->width = width;
    this->height = height;
    this->camera = cam;
    this->camera_orientation = glm::mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    this->angle = ANGLE;
    to_coord();
};

void Projection::set_camera(glm::vec3 camera){
    this->camera = camera;
};
        
void Projection::set_angle(float angle){
    this->angle = angle;
};

float Projection::project(float pos, float index, float z){
    float s1 = FOCAL_LENGTH*PROJ_SCALE_FACTOR;
    float s2 = (pos - camera[index]);
    float s3 = z - camera[2];
    float s4 = (s2/s3) * s1;
    // std::cout << s4 << " and " << s3 << " and " << s2 << " and " << s1 << " and" << angle  << std::endl;
    return int(round(s4));
}

float Projection::hypotenuse(glm::vec3 v1, glm::vec3 v2){
    float dx = v1[0] - v2[0];
    float dy = v1[1] - v2[1];
    float dz = v1[2] - v2[2];
    return sqrt((dx*dx) + (dy*dy) + (dz*dz));
};

CanvasPoint Projection::project_vertex(glm::vec3 v){ 

    glm::vec3 camToVertex = v - camera;
    // float length = hypotenuse(v,camera);
    glm::vec3 adjustedVector = (camera_orientation*camToVertex) + camera;
    // std::cout << "--NEW--" << std::endl;
    // print_vec3(camera);
    // print_vec3(camToVertex);
    // print_vec3(adjustedVector);

    float xi = project(adjustedVector[0], 0, adjustedVector[2]);
    float yi = project(adjustedVector[1], 1, adjustedVector[2]);

    CanvasPoint v_image(xi + (width/2) , yi + (height/2) , adjustedVector[2]);
    
    return v_image;
};

CanvasTriangle Projection::project_model_triangle(ModelTriangle model_triangle, bool texture){
    CanvasTriangle projected_triangle;
    CanvasPoint v1 = project_vertex(model_triangle.vertices[0]);
    CanvasPoint v2 = project_vertex(model_triangle.vertices[1]);
    CanvasPoint v3 = project_vertex(model_triangle.vertices[2]);
    if(texture){
        v1.texturePoint = model_triangle.texturePoints[0];
        v2.texturePoint = model_triangle.texturePoints[1];
        v3.texturePoint = model_triangle.texturePoints[2];
    }
    projected_triangle = CanvasTriangle(v1, v2, v3);
    return projected_triangle;
}

void print_matrix(glm::mat3 m){
    std::cout << " " << std::endl;
    for (size_t i = 0; i < 3; i++)
    {
        std::cout << m[i][0] << "|" << m[i][1] << "|" << m[i][2] << std::endl;
    }
}

void Projection::rotate_camera_y(bool dir){
    float temp_angle = glm::radians(-1*angle);
    if(dir){
        temp_angle = glm::radians(angle);
    }
    glm::mat3 roty(cosf(temp_angle), 0, -1*sinf(temp_angle),
                  0, 1, 0,
                  sinf(temp_angle), 0, cosf(temp_angle));

    // glm::mat3 rotz(cosf(temp_angle), sinf(temp_angle), 0,
    //               -1*sinf(temp_angle), cosf(temp_angle), 0,
    //               0, 0, 1);

    camera = roty*camera;

}

void Projection::rotate_camera_x(bool dir){

    float temp_angle = glm::radians(-1*angle);
    if(dir){
        temp_angle = glm::radians(angle);
    }
   
    glm::mat3 rotx(1, 0, 0,
                0, cos(temp_angle), -1*sin(temp_angle),
                0, sin(temp_angle), cos(temp_angle));
    
    camera = rotx*camera;
    
}


void Projection::pan_camera_y(bool dir){
    float temp_angle = glm::radians(-1*angle);
    if(dir){
        temp_angle = glm::radians(angle);
    }
    glm::mat3 roty(cosf(temp_angle), 0, -1*sinf(temp_angle),
                  0, 1, 0,
                  sinf(temp_angle), 0, cosf(temp_angle));

    camera_orientation = camera_orientation*roty;
}

void Projection::tilt_camera_x(bool dir){
    float temp_angle = glm::radians(-1*angle);
    if(dir){
        temp_angle = glm::radians(angle);
    }
   
    glm::mat3 rotx(1, 0, 0,
                0, cos(temp_angle), -1*sin(temp_angle),
                0, sin(temp_angle), cos(temp_angle));
    
    camera_orientation = camera_orientation*rotx;
    print_matrix(camera_orientation);

}

void Projection::trans_camera(float x, float y, float z){
    glm::vec3 tran(x, y, z);
    tran = camera_orientation*tran;
    camera = camera + tran;
}

void Projection::lookaty(){
    glm::vec3 forward = glm::normalize(camera - center);
    glm::vec3 tmp(0, 1, 0);
    glm::vec3 right = glm::cross(forward, tmp);
    glm::vec3 up = glm::cross(right, forward);
    camera_orientation = glm::mat3(right[0], right[1], right[2],
                           up[0], up[1], up[2],
                           forward[0], forward[1], forward[2]);
};

void Projection::lookatx(){
    glm::vec3 forward = glm::normalize(camera - center);
    glm::vec3 tmp(1, 0, 0);
    glm::vec3 up = glm::cross(tmp, forward);
    glm::vec3 right = glm::cross(up, forward);
    camera_orientation = glm::mat3(right[0], right[1], right[2],
                           up[0], up[1], up[2],
                           forward[0], forward[1], forward[2]);
};

void Projection::draw_projected_model(DrawingWindow &window){

    std::vector<CanvasPoint> window_points(width*height);
    std::vector<uint32_t> window_colours(width*height);

    for (size_t i = 0; i < width*height; i++)
    {
        window_colours[i] = 0;
        window_points[i] = CanvasPoint(0,0,-1*INFINITY);
    }

    std::string current_texture_path = "";
    TextureMap current_texture;
    
    for (ObjFileObject o: model_triangles)
    {
        for(ModelTriangle v: o.tris){

            CanvasTriangle projected_triangle = project_model_triangle(v, o.texture);

            bool skip_tri = false;
            for (size_t i = 0; i < 3; i++)
            {
                if(abs(projected_triangle.vertices[i].x) > 2*width || abs(projected_triangle.vertices[i].y) > 2*height ){skip_tri = true;}
            }
            if(skip_tri){continue;}
            
            uint32_t color = (255 << 24) + (v.colour.red << 16) + (v.colour.green << 8) + v.colour.blue;

            if(o.texture){
                if(current_texture_path != o.tpath){
                        current_texture_path = o.tpath;
                        current_texture = texture_map_dict[o.tpath];
                }
            }
            
            std::vector<CanvasPoint> points = draw_tri_solid(projected_triangle);

            for (size_t i = 0; i < points.size(); i++)
            {
                CanvasPoint p = points[i];
                if(o.texture){
                    color = current_texture.pixels[(current_texture.width * p.texturePoint.y) + p.texturePoint.x];
                }
                if((p.y*width) + p.x >= width * height || (p.y*width) + p.x < 0 ){continue;}
                float current_depth = window_points[(p.y*width) + p.x].depth;
                if(current_depth <= p.depth){
                    window_points[(p.y*width) + p.x] = p;
                    window_colours[(p.y*width) + p.x] = color;
                }
            }

        }
        
    }

    for (size_t i = 0; i < width*height; i++)
    {
        window.setPixelColour(window_points[i].x, window_points[i].y, window_colours[i]);
    }
    
};