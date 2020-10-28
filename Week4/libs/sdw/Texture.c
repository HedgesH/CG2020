#include "Texture.h"

std::vector<float> interpolatef(float start, float end, int size){
    float step_length = (end-start)/(size-1);
    std::vector<float> result(size);
    for (size_t i = 0; i < size; i++){result[size] = start + (step_length*i);}
    return result;
};

std::vector<CanvasPoint> interpolateCanvasPoint(CanvasPoint start, CanvasPoint end, int size){
    float step_lengthx = (end.x-start.x)/(size-1);
    float step_lengthy = (end.y-start.y)/(size-1);
    std::vector<CanvasPoint> result(size);
    for (size_t i = 0; i < size; i++){result[size] = CanvasPoint(start.x + (step_lengthx*i), start.y + (step_lengthy*i))}
    return result;
};

std::vector<TexturePoint> interpolateTexturePoint(CanvasPoint start, CanvasPoint end, int size){
    float step_lengthx = (end.x-start.x)/(size-1);
    float step_lengthy = (end.y-start.y)/(size-1);
    std::vector<TexturePoint> result(size);
    for (size_t i = 0; i < size; i++){result[size] = TexturePoint(start.x + (step_lengthx*i), start.y + (step_lengthy*i))}
    return result;
}

std::vector<CanvasPoint> interpolateTextureAndCanvasPoint(CanvasPoint start, CanvasPoint end, int size){
    float step_lengthx = (end.x-start.x)/(size-1);
    float step_lengthy = (end.y-start.y)/(size-1);
    float texture_step_lengthx = (end.texturePoint.x-start.texturePoint.x)/(size-1);
    float texture_step_lengthy = (end.texturePoint.y-start.texturePoint.y)/(size-1);
    std::vector<TexturePoint> result(size);
    for (size_t i = 0; i < size; i++){
        result[size] = CanvasPoint(start.x + (step_lengthx*i), start.y + (step_lengthy*i))
        result[size].texturePoint = TexturePoint(start.texturePoint.x + (texture_step_lengthx*i),start.texturePoint.y + (texture_step_lengthy*i)  )
    }
    return result;
}

float hypotenuse(CanvasPoint x, CanvasPoint y){
    return sqrt(((y.y-x.y)*(y.y-x.y)) + ((y.x-x.x)*(y.x-x.x)));
}

float hypotenuse(TexturePoint x, TexturePoint y){
    return sqrt(((y.y-x.y)*(y.y-x.y)) + ((y.x-x.x)*(y.x-x.x)));
}

std::vector<CanvasPoint> line(CanvasPoint start, CanvasPoint end){
    int size = int(hypotenuse(start, end));
    return interpolateCanvasPoint(start, end, size);
}

std::vector<TexturePoint> texture_line(TexturePoint start, TexturePoint end){
    int size = int(hypotenuse(start, end));
    return interpolateTexturePoint(start, end, size);
}

void sort_triangle(CanvasTriangle &tri){
    CanvasPoint v1,v2,v3;
    v1 = tri.v0;
    v2 = tri.v1;
    v3 = tri.v2;
    std::vector<CanvasPoint> v(v1, v2, v3);
    std::sort(std::begin(v), 
              std::end(v), 
              [](CanvasPoint a, CanvasPoint b) {return a.y > b.y; });
    tri.v0 = v[0];
    tri.v1 = v[1];
    tri.v2 = v[2];
}

void new_middle_texture_point(CanvasPoint top, CanvasPoint bottom, CanvasPoint &new_middle){
    float length = hypotenuse(top, bottom);
    float top_to_middle = hypotenuse(top, new_middle);
    float prop = top_to_middle/length;

    float texture_dx = top.texturePoint.x - bottom.texturePoint.x;
    float texture_dy = top.texturePoint.y - bottom.texturePoint.y;

    float new_middle_tp_x = bottom.texturePoint.x + (prop*texture_dx);
    float new_middle_tp_y = bottom.texturePoint.y + (prop*texture_dy);

    new_middle.texturePoint = TexturePoint(new_middle_tp_x,new_middle_tp_y);
}

void split_triangle(CanvasTriangle ord_tri, CanvasPoint &left_middle, CanvasPoint &right_middle){
    CanvasPoint bottom = ord_tri[2];
    CanvasPoint middle = ord_tri[1];
    CanvasPoint top = ord_tri[0];
    CanvasPoint new_middle;
    if(bottom.x == top.x){
        new_middle = CanvasPoint(bottom.x,middle.y);
    }
    else{
        float m = (bottom.y - top.y) / (bottom.x - top.x);
        float c = top.y - (m*top.x);
        float new_middle_x = (yoffset-c)/m;
        new_middle = CanvasPoint(new_middle_x,middle.y); 
    }
    new_middle_texture_point(top, bottom, new_middle);
    if(new_middle.x <= middle.x){
        left_middle = new_middle;
        right_middle = middle;
    }
    else{
        right_middle = new_middle;
        left_middle = middle;
    }
}

std::vector<CanvasPoint> fill_top_triangle(CanvasPoint top, CanvasPoint lm, CanvasPoint rm){
    int height = lm.y - top.y;
    std::vector<float> top_to_leftx = interpolate(top.x, lm.x, height);
    std::vector<float> top_to_rightx = interpolate(top.x, rm.x, height);
    std::vector<float> texture_top_to_leftx = interpolate(top.texturePoint.x, lm.texturePoint.x, height);
    std::vector<float> texture_top_to_rightx = interpolate(top.texturePoint.x, rm.texturePoint.x, height);

    float texture_left_ratio = (lm.texturePoint.y - top.texturePoint.y - ) / (height-1);
    float texture_right_ratio = (rm.texturePoint.y - top.texturePoint.y - ) / (height-1);

    std::vector<CanvasPoint> points();
    for (int i = 0; i < height; i++)
    {
        float leftx = top_to_left[i];
        float lefty = i;

        float rightx = top_to_right[i];
        float righty = i;

        float texture_leftx = texture_top_to_left[i];
        float texture_lefty = i*texture_left_ratio;
        
        float texture_rightx = texture_top_to_right[i];
        float texture_righty = i*texture_right_ratio;

        CanvasPoint left(leftx, lefty);
        left.texturePoint = TexturePoint(texture_leftx, texture_lefty);
        CanvasPoint right(rightx, righty);
        right.texturePoint = TexturePoint(texture_rightx, texture_righty);
        int size = int(abs(right.x-left.x));
        std::vector<CanvasPoint> row_points = interpolateTextureAndCanvasPoint(left, right, size);
        points.insert(points.end(), row_points.begin(), row_points.end());
    }

    return points;
    

}

std::vector<CanvasPoint> fill_bottom_triangle(CanvasPoint bottom, CanvasPoint lm, CanvasPoint rm){
    int height = bottom.h - lm.h;
    std::vector<float> bottom_to_leftx = interpolate(bottom.x, lm.x, height);
    std::vector<float> bottom_to_rightx = interpolate(bottom.x, rm.x, height);
    std::vector<float> texture_bottom_to_leftx = interpolate(bottom.texturePoint.x, lm.texturePoint.x, height);
    std::vector<float> texture_bottom_to_rightx = interpolate(bottom.texturePoint.x, rm.texturePoint.x, height);

    float texture_left_ratio = (bottom.texturePoint.y - lm.texturePoint.y) / (height-1);
    float texture_right_ratio = (bottom.texturePoint.y - rm.texturePoint.y) / (height-1);

    std::vector<CanvasPoint> points();
    for (int i = 0; i < height; i++)
    {
        float leftx = bottom_to_left[i];
        float lefty = i;

        float rightx = bottom_to_right[i];
        float righty = i;

        float texture_leftx = texture_bottom_to_left[i];
        float texture_lefty = i*texture_left_ratio;
        
        float texture_rightx = texture_bottom_to_right[i];
        float texture_righty = i*texture_right_ratio;

        CanvasPoint left(leftx, lefty);
        left.texturePoint = TexturePoint(texture_leftx, texture_lefty);
        CanvasPoint right(rightx, righty);
        right.texturePoint = TexturePoint(texture_rightx, texture_righty);
        int size = int(abs(right.x-left.x));
        std::vector<CanvasPoint> row_points = interpolateTextureAndCanvasPoint(left, right, size);
        points.insert(points.end(), row_points.begin(), row_points.end());
    }

    return points;
}

std::vector<CanvasPoint> fill_triangle(CanvasTriangle tri){
    sort_triangle(tri);
    CanvasPoint top = tri[0];
    CanvasPoint bottom = tri[2];
    CanvasPoint lm,rm;
    split_triangle(tri, lm, rm);
    std::vector<CanvasPoint> filled_tri1 = fill_top_triangle(top, lm, rm);
    std::vector<CanvasPoint> filled_tri2 = fill_top_triangle(bottom, lm, rm);
    filled_tri1.insert(filled_tri1.end(), filled_tri2.begin(), filled_tri2.end());
    return filled_tri1;
}

void draw_tri_texture(DrawingWindow &window, CanvasTriangle tri, std::string texture_path){
    std::vector<CanvasPoint> filled_tri = fill_triangle(tri);
    TextureMap map = TextureMap("/Users/harryhedges 1/Desktop/computer science/CG/CG2020/resources/texture.ppm");
    uint32_t color;
    for (CanvasPoint element: filled_tri){
        color = bricks.pixels[(bricks.width * element.texturePoint.y) + element.texturePoint.x];
        window.setPixelColour(element.x, element.y, color);
    }
}