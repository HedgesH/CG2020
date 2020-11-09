#include "Week3.h"

//LINE

std::vector<glm::vec2> interpolatevec2(glm::vec2 start, glm::vec2 end, int size){
    glm::vec2 interval = (end - start) / float(size - 1);
    std::vector<glm::vec2> interpolations(size);
    for (size_t i = 0; i < size; i++){interpolations[i] = start + (interval*float(i));};
    return interpolations;
};

std::vector<CanvasPoint> interpolateCanvasPoint(CanvasPoint start, CanvasPoint end, int size){
    glm::vec2 startv(start.x,start.y);
    glm::vec2 endv(end.x,end.y);
    std::vector<glm::vec2> points = interpolatevec2(startv, endv, size);
    std::vector<CanvasPoint> canvas_points(size);
    for (size_t i = 0; i < size; i++)
    {
        CanvasPoint temp(points[i][0], points[i][1]);
        canvas_points[i] = temp;
    }
    return canvas_points;
};

std::vector<float> interpolateSingleFloat(float start, float end, int size){
    float interval = (end - start) / (size);
    float interpolations[size];
    for (int i = 0; i < size; i++)
    {
        interpolations[i] = start + (interval*i);
        
    };
    
    return std::vector<float>(interpolations, interpolations + size);
};

int hypotenuse(CanvasPoint start, CanvasPoint end){
    int length = (end.y - start.y)*(end.y - start.y);
    length = length + (end.x - start.x)*(end.x - start.x);
    return int(round(sqrt(length)));
};

float hypotenusef(CanvasPoint start, CanvasPoint end){
    float length = (end.y - start.y)*(end.y - start.y);
    length = length + (end.x - start.x)*(end.x - start.x);
    return sqrt(length);
};

int hypotenuse(TexturePoint start, TexturePoint end){
    int length = (end.y - start.y)*(end.y - start.y);
    length = length + (end.x - start.x)*(end.x - start.x);
    return int(round(sqrt(length)));
};

std::vector<CanvasPoint> line(CanvasPoint start, CanvasPoint end){
    int size = hypotenuse(start, end);
    std::vector<CanvasPoint> line_points = interpolateCanvasPoint(start, end, size);
    return line_points;
};

void draw_line(DrawingWindow &window, CanvasPoint start, CanvasPoint end, glm::vec3 color_vector){
    std::vector<CanvasPoint> line_points = line(start, end);
    uint32_t color = (255 << 24) + (int(color_vector[0]) << 16) + (int(color_vector[1]) << 8) + int(color_vector[2]);
    for (CanvasPoint element : line_points) {window.setPixelColour(element.x, element.y, color);};
};

//UNFILLED TRIANGLE

std::vector<CanvasPoint> unfilled_triangle(CanvasPoint p1, CanvasPoint p2, CanvasPoint p3){
    std::vector<CanvasPoint> l1 = line(p1, p2);
    std::vector<CanvasPoint> l2 = line(p2, p3);
    std::vector<CanvasPoint> l3 = line(p3, p1);
    l1.insert(l1.end(), l2.begin(), l2.end());
    l1.insert(l1.end(), l3.begin(), l3.end());
    return l1;
};

void draw_unfilled_triangle(DrawingWindow &window, CanvasPoint p1, CanvasPoint p2, CanvasPoint p3, glm::vec3 color_vector){
    std::vector<CanvasPoint> triangle_points = unfilled_triangle(p1, p2, p3);
    uint32_t color = (255 << 24) + (int(color_vector[0]) << 16) + (int(color_vector[1]) << 8) + int(color_vector[2]);
    for (CanvasPoint element : triangle_points) {window.setPixelColour(element.x, element.y, color);};
};

void draw_random_unfilled_triangle(DrawingWindow &window){
    std::vector<CanvasPoint> triangle_pos(3);
    for (size_t i = 0; i < 3; i++)
    {
        CanvasPoint pos(rand() % window.width, rand() % window.height);
        triangle_pos[i] = pos;
    };
    glm::vec3 color(rand() % 256, rand() % 256, rand() % 256);
    draw_unfilled_triangle(window, triangle_pos[0], triangle_pos[1], triangle_pos[2], color);
};

//FILLED TRIANGLE

std::vector<CanvasPoint> triangle_sort(CanvasPoint p1, CanvasPoint p2, CanvasPoint p3){
    std::vector<CanvasPoint> sorted_points(3);
    sorted_points[0] = p1;
    sorted_points[1] = p2;
    sorted_points[2] = p3;

    std::sort(std::begin(sorted_points), 
              std::end(sorted_points), 
              [](CanvasPoint a, CanvasPoint b) {return a.y > b.y; });
    return sorted_points;

};

float intersection_line(CanvasPoint top, CanvasPoint bottom , int yoffset){
    if(bottom.x == top.x) return bottom.x;
    float m = (bottom.y - top.y) / (bottom.x - top.x);
    float c = top.y - (m*top.x);
    return round(((yoffset-c)/m));
};

std::vector<CanvasPoint> fill_flat_triangle(CanvasPoint bl, CanvasPoint br, CanvasPoint other){
    std::vector<float> increments = interpolateSingleFloat(bl.y, other.y,int(round(abs(bl.y-other.y))));
    
    std::vector<CanvasPoint> filled_triangle = unfilled_triangle(bl, br, other);

    for(float increment: increments){
        CanvasPoint lstart(intersection_line(bl, other, increment), increment);        
        CanvasPoint rend(intersection_line(br, other, increment), increment);
        std::vector<CanvasPoint> fill = interpolateCanvasPoint(lstart, rend, int(abs(rend.x - lstart.x)));
        filled_triangle.insert(filled_triangle.end(), fill.begin(), fill.end());
    };
    return filled_triangle;
}

std::vector<CanvasPoint> fill_triangle(CanvasPoint p1, CanvasPoint p2, CanvasPoint p3){
    
    std::vector<CanvasPoint> sorted_triangle = triangle_sort(p1, p2, p3);

    CanvasPoint split_point(intersection_line(sorted_triangle[0], sorted_triangle[2], sorted_triangle[1].y), sorted_triangle[1].y);

    // std::vector<glm::vec2> l1 = line(sorted_triangle[1], split_point);
    // std::vector<glm::vec2> t2 = unfilled_triangle(p2, p1, p3);
    // l1.insert(l1.end(), t2.begin(), t2.end());
    // return l1;
    
    if(split_point.x <= p2.x){
        std::vector<CanvasPoint> t1 = fill_flat_triangle(split_point, sorted_triangle[1], sorted_triangle[0]);
        std::vector<CanvasPoint> t2 = fill_flat_triangle(split_point, sorted_triangle[1], sorted_triangle[2]);
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;
    }
    else{
        std::vector<CanvasPoint> t1 = fill_flat_triangle(sorted_triangle[1], split_point, sorted_triangle[0]);
        std::vector<CanvasPoint> t2 = fill_flat_triangle(sorted_triangle[1], split_point, sorted_triangle[2]);
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;
    }
}

void draw_random_filled_triangle(DrawingWindow &window){
    std::vector<CanvasPoint> triangle_pos(3);
    
    for (size_t i = 0; i < 3; i++)
    {
        CanvasPoint pos(rand() % window.width, rand() % window.height);
        triangle_pos[i] = pos;
    };
    
    glm::vec3 color_vector(rand() % 256, rand() % 256, rand() % 256);
    uint32_t color = (255 << 24) + (int(color_vector[0]) << 16) + (int(color_vector[1]) << 8) + int(color_vector[2]);
    std::vector<CanvasPoint> triangle_points = fill_triangle(triangle_pos[0], triangle_pos[1], triangle_pos[2]);

    for(CanvasPoint element: triangle_points){
        window.setPixelColour(element.x,element.y, color);
    }

    std::vector<CanvasPoint> triangle_points2 = unfilled_triangle(triangle_pos[0], triangle_pos[1], triangle_pos[2]);
    color = (255 << 24) + (255 << 16) + (255 << 8) + 255;
    for(CanvasPoint element: triangle_points2){
        window.setPixelColour(element.x,element.y, color);
    }
}

//

std::vector<CanvasPoint> interpolateTexturePoint(CanvasPoint start, CanvasPoint end, int size){
    glm::vec2 startv(start.x,start.y);
    glm::vec2 endv(end.x,end.y);
    glm::vec2 tstartv(start.texturePoint.x, start.texturePoint.y);
    glm::vec2 tendv(end.texturePoint.x, end.texturePoint.y);
    std::vector<glm::vec2> points = interpolatevec2(startv, endv, size);
    std::vector<glm::vec2> tpoints = interpolatevec2(tstartv, tendv, size);

    // std::cout << tstartv.x << " " << tstartv.y << '\n';
    // std::cout << tendv.x << " " << tendv.y << '\n';

    std::vector<CanvasPoint> canvas_points(size);
    for (size_t i = 0; i < size; i++)
    {
        CanvasPoint temp(points[i][0], points[i][1]);
        temp.texturePoint.x = tpoints[i][0];
        temp.texturePoint.y = tpoints[i][1];
        canvas_points[i] = temp;
    }
    return canvas_points;
};

float intersection_line(TexturePoint top, TexturePoint bottom , int yoffset){
    if(bottom.x == top.x) return bottom.x;
    float m = (bottom.y - top.y) / (bottom.x - top.x);
    float c = top.y - (m*top.x);
    return round(((yoffset-c)/m));
};

std::vector<TexturePoint> triangle_sort(TexturePoint p1, TexturePoint p2, TexturePoint p3){
    std::vector<TexturePoint> sorted_points(3);
    sorted_points[0] = p1;
    sorted_points[1] = p2;
    sorted_points[2] = p3;

    std::sort(std::begin(sorted_points), 
              std::end(sorted_points), 
              [](TexturePoint a, TexturePoint b) {return a.y > b.y; });
    return sorted_points;

};

TexturePoint mapTexture(CanvasPoint p1, CanvasPoint p2, CanvasPoint p3){
    double length = hypotenusef(p1,p3);
    double p1p2 = hypotenusef(p1,p2);
    // float prop= p1p2/length;


    double diffx = (p1p2*(p3.texturePoint.x-p1.texturePoint.x))/length + p1.texturePoint.x;
    double diffy = (p1p2*(p3.texturePoint.y-p1.texturePoint.y))/length + p1.texturePoint.y;
    TexturePoint ans(diffx,diffy);

    if(diffx > 400 || diffy > 395){
        std::cout << '\n';
        std::cout << length << " " << p1p2 << "\n";
        std::cout << p1.x << " " << p1.y << " " << p1.texturePoint.x << " " << p1.texturePoint.y << "\n";
        std::cout << p2.x << " " << p2.y << " " << p2.texturePoint.x << " " << p2.texturePoint.y << "\n";
        std::cout << p3.x << " " << p3.y << " " << p3.texturePoint.x << " " << p3.texturePoint.y << "\n";
        std::cout << " " << diffx << " "<< diffy << "\n";

    }

    return ans;
}

std::vector<CanvasPoint> fill_flat_texture(CanvasPoint bl, CanvasPoint br, CanvasPoint other){
    std::vector<float> xsteps1 = interpolateSingleFloat(other.x,bl.x,int(abs(other.y-bl.y)));
    std::vector<float> xsteps2 = interpolateSingleFloat(other.x,br.x,int(abs(other.y-br.y)));
    std::vector<float> xtsteps1 = interpolateSingleFloat(other.texturePoint.x,bl.texturePoint.x,int(abs(other.y-bl.y)));
    std::vector<float> xtsteps2 = interpolateSingleFloat(other.texturePoint.x,br.texturePoint.x,int(abs(other.y-br.y)));

    float yratio1 = (other.texturePoint.y - bl.texturePoint.y)/abs(other.y-bl.y);
    float yratio2 = (other.texturePoint.y - br.texturePoint.y)/abs(other.y-bl.y);
    
    std::vector<CanvasPoint> filled_triangle = unfilled_triangle(bl, br, other);

    for (size_t i = 0; i < int(round(abs(bl.y-other.y))); i++)
    {
        float startx = xsteps1[i];
        float endx = xsteps2[i];
        CanvasPoint start(xsteps1[i], i);
        CanvasPoint end(xsteps2[i], i);
        start.texturePoint = TexturePoint(xtsteps1[i], i * yratio1);
        end.texturePoint = TexturePoint(xtsteps2[i], i * yratio2);
        
        std::vector<CanvasPoint> fill = interpolateTexturePoint(start,end, int(abs(end.x-start.x)));
        filled_triangle.insert(filled_triangle.end(), fill.begin(), fill.end());
    }
    

  
    return filled_triangle;
}

std::vector<CanvasPoint> fill_texture(CanvasPoint p1, CanvasPoint p2, CanvasPoint p3){
    
    std::vector<CanvasPoint> sorted_triangle = triangle_sort(p1, p2, p3);


    CanvasPoint split_point(intersection_line(sorted_triangle[0], sorted_triangle[2], sorted_triangle[1].y), sorted_triangle[1].y);
    split_point.texturePoint = mapTexture(sorted_triangle[0], split_point, sorted_triangle[2]);

    // std::vector<glm::vec2> l1 = line(sorted_triangle[1], split_point);
    // std::vector<glm::vec2> t2 = unfilled_triangle(p2, p1, p3);
    // l1.insert(l1.end(), t2.begin(), t2.end());
    // return l1;

    if(split_point.x <= p2.x){
        std::vector<CanvasPoint> t1 = fill_flat_texture(split_point, sorted_triangle[1], sorted_triangle[0]);
        std::vector<CanvasPoint> t2 = fill_flat_texture(split_point, sorted_triangle[1], sorted_triangle[2]);
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;
    }
    else{
        std::vector<CanvasPoint> t1 = fill_flat_texture(sorted_triangle[1], split_point, sorted_triangle[0]);
        std::vector<CanvasPoint> t2 = fill_flat_texture(sorted_triangle[1], split_point, sorted_triangle[2]);
        t1.insert(t1.end(), t2.begin(), t2.end());
        return t1;     
    }
}

void draw_texture_triangle(DrawingWindow &window, std::vector<CanvasPoint> triangle_pos){
    // TextureMap bricks("/Users/harryhedges 1/Downloads/blue-and-red-paper-layers-abstract-background-vector-3414579.PPM");
    TextureMap bricks("/Users/harryhedges 1/Desktop/computer science/CG/CG2020/resources/texture.ppm");

    
    
    std::vector<CanvasPoint> triangle_points = fill_texture(triangle_pos[0], triangle_pos[1], triangle_pos[2]);
    uint32_t color;
    for(CanvasPoint element: triangle_points){
        // std::cout << element.texturePoint.x << '\n';
        color = bricks.pixels[((element.texturePoint.y)*bricks.width) + element.texturePoint.x];
        // color = bricks.pixels[0];
        window.setPixelColour(element.x,element.y, color);
        // color = (255 << 24) + (int(255) << 16) + (int(255) << 8) + int(255);
        // window.setPixelColour(element.texturePoint.x,element.texturePoint.y, color);
    }

}



