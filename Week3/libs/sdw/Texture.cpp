#include "Texture.h"

std::vector<int> interpolatef(float start, float end, int size){
    float step_length = float((end-start))/(size-1);
  
    std::vector<int> result(size);
    for (size_t i = 0; i < size; i++){result[i] = int(round(start + (step_length*i)));}
    return result;
};

std::vector<CanvasPoint> interpolateCanvasPoint(CanvasPoint start, CanvasPoint end, int size){
    float step_lengthx = (end.x-start.x)/(size-1);
    float step_lengthy = (end.y-start.y)/(size-1);
    std::vector<CanvasPoint> result(size);
    for (size_t i = 0; i < size; i++){result[i] = CanvasPoint(roundl(start.x + (step_lengthx*i)), roundl(start.y + (step_lengthy*i)));}
    return result;
    
};

std::vector<TexturePoint> interpolateTexturePoint(TexturePoint start, TexturePoint end, int size){
    float step_lengthx = (end.x-start.x)/(size-1);
    float step_lengthy = (end.y-start.y)/(size-1);
    std::vector<TexturePoint> result(size);
    for (size_t i = 0; i < size; i++){result[i] = TexturePoint(roundl(start.x + (step_lengthx*i)), roundl(start.y + (step_lengthy*i)));}
    return result;
}

std::vector<CanvasPoint> interpolateTextureAndCanvasPoint(CanvasPoint start, CanvasPoint end, int size){
    float step_lengthx = (end.x-start.x)/(size-1);
    float step_lengthy = (end.y-start.y)/(size-1);
    float texture_step_lengthx = (end.texturePoint.x-start.texturePoint.x)/(size-1);
    float texture_step_lengthy = (end.texturePoint.y-start.texturePoint.y)/(size-1);
    std::vector<CanvasPoint> result(size);
    for (size_t i = 0; i < size; i++){
        result[i] = CanvasPoint(roundl(start.x + (step_lengthx*i)), roundl(start.y + (step_lengthy*i)));
        result[i].texturePoint = TexturePoint(roundl(start.texturePoint.x + (texture_step_lengthx*i)),roundl(start.texturePoint.y + (texture_step_lengthy*i)));
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

CanvasTriangle sort_triangle(CanvasTriangle &tri){
    CanvasPoint v1,v2,v3;
    v1 = tri.v0();
    v2 = tri.v1();
    v3 = tri.v2();
    std::vector<CanvasPoint> v(3);
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
    std::sort(std::begin(v), 
              std::end(v), 
              [](CanvasPoint a, CanvasPoint b) {return a.y > b.y; });
    return CanvasTriangle(v[0], v[1], v[2]);
}

void new_middle_texture_point(CanvasPoint top, CanvasPoint bottom, CanvasPoint &new_middle){
    float length = hypotenuse(top, bottom);
    float top_to_middle = hypotenuse(top, new_middle);
    float prop = top_to_middle/length;

    float texture_dx = bottom.texturePoint.x - top.texturePoint.x  ;
    float texture_dy = bottom.texturePoint.y - top.texturePoint.y ;

    int new_middle_tp_x = round(top.texturePoint.x + (prop*texture_dx));
    int new_middle_tp_y = round(top.texturePoint.y + (prop*texture_dy));
    
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
        float new_middle_x = (middle.y-c)/m;
        new_middle = CanvasPoint(round(new_middle_x),middle.y); 
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

std::vector<CanvasPoint> fill_triangle_points(CanvasPoint p, CanvasPoint lm, CanvasPoint rm){
    int dy = abs(p.y-lm.y);
    std::vector<int> increments = interpolatef(p.y, lm.y, dy+1);
    std::vector<int> p_to_leftx = interpolatef(p.x, lm.x, dy);
    std::vector<int> p_to_rightx = interpolatef(p.x, rm.x, dy);
    std::vector<int> texture_p_to_leftx = interpolatef(p.texturePoint.x, lm.texturePoint.x, dy);
    std::vector<int> texture_p_to_rightx = interpolatef(p.texturePoint.x, rm.texturePoint.x, dy);

    float texture_left_ratio = (lm.texturePoint.y - p.texturePoint.y) / (dy-1);
    float texture_right_ratio = (rm.texturePoint.y - p.texturePoint.y) / (dy-1);
    
    std::vector<CanvasPoint> points;
    for (size_t i = 0; i < dy; i++)
    {
        int increment = increments[i];
        CanvasPoint left(p_to_leftx[i], increment);
        left.texturePoint = TexturePoint(texture_p_to_leftx[i], p.texturePoint.y + roundl(i*texture_left_ratio));
        CanvasPoint right(p_to_rightx[i], increment);
        right.texturePoint = TexturePoint(texture_p_to_rightx[i], p.texturePoint.y + roundl(i*texture_right_ratio));
        int size = int(abs(right.x-left.x));
        std::vector<CanvasPoint> row_points = interpolateTextureAndCanvasPoint(left, right, size*2);
        points.insert(points.end(), row_points.begin(), row_points.end());
    }

    return points;
}

std::vector<CanvasPoint> fill_triangle(CanvasTriangle tri){
    tri = sort_triangle(tri);
    CanvasPoint top = tri.v2();
    CanvasPoint bottom = tri.v0();
    CanvasPoint lm,rm;
    split_triangle(tri, lm, rm);
    std::vector<CanvasPoint> points = interpolateTextureAndCanvasPoint(lm, rm, rm.x - lm.x);
    std::vector<CanvasPoint> filled_tri1 = fill_triangle_points(top, lm, rm);
    std::vector<CanvasPoint> filled_tri2 = fill_triangle_points(bottom, lm, rm);
    points.insert(points.end(), filled_tri1.begin(), filled_tri1.end());
    points.insert(points.end(), filled_tri2.begin(), filled_tri2.end());
    return points;
}

void draw_tri_texture(DrawingWindow &window, CanvasTriangle tri, std::string texture_path){
    std::vector<CanvasPoint> filled_tri = fill_triangle(tri);
    TextureMap map = TextureMap(texture_path);
    uint32_t color;

    for (CanvasPoint element: filled_tri){
        color = map.pixels[(map.width * element.texturePoint.y) + element.texturePoint.x];
        
        window.setPixelColour(element.x, element.y, color);
    }
}

void draw_tri_solid(DrawingWindow &window, CanvasTriangle tri){
    CanvasPoint p1(tri.vertices[0].x, tri.vertices[0].y)
    p1.texturePoint = TexturePoint(tri.vertices[0].x, tri.vertices[0].y);
    CanvasPoint p2(tri.vertices[1].x, tri.vertices[1].y);
    p2.texturePoint = TexturePoint(tri.vertices[1].x, tri.vertices[1].y);
    CanvasPoint p3(tri.vertices[2].x, tri.vertices[2].y);
    p3.texturePoint = TexturePoint(tri.vertices[2].x, tri.vertices[2].y);
    CanvasTriangle new_tri(p1, p2, p3);
    std::vector<CanvasPoint> filled_tri = fill_triangle(new_tri);
    uint32_t color = tri.colour;
    for (CanvasPoint element: filled_tri){
        window.setPixelColour(element.x, element.y, color);
    }
}