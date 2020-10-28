#include "Week2.h"


std::vector<float> interpolateSingleFloat(float start, float end, int size){
    float interval = (end - start) / size;
    float interpolations[size];

    for (int i = 0; i < size; i++)
    {
        interpolations[i] = start + (interval*i);
    };

    return std::vector<float>(interpolations, interpolations + size);
};

std::vector< std::vector<uint32_t>> gradient_box(int height, int width){

    std::vector< std::vector<uint32_t>> horizontal(width);
    
    std::vector<float> color = interpolateSingleFloat(0,255,width);

    for (int i = 0; i < width; i++) {
        
        uint32_t color2 = color.at(width - i - 1);
        std::vector<uint32_t> vertical(height);
        
        for (int j = 0; j < height; j++) {
            vertical.at(j) = color2;
        };
        
        horizontal.at(i) = vertical;

    }

    return horizontal;
    
};

std::vector<glm::vec3> interpolatevec3(glm::vec3 start, glm::vec3 end, int size){
    glm::vec3 interval = (end - start) / float(size);
    std::vector<glm::vec3> interpolations(size);

    for (size_t i = 0; i < size; i++)
    {
        interpolations[i] = start + (interval*float(i));
    };

    return interpolations;
    
};

std::vector<std::vector<glm::vec3>> color_grad(int height, int width){

    std::vector< std::vector<glm::vec3>> vertical(height);
    std::vector<glm::vec3> colstart = interpolatevec3(glm::vec3(255, 0 , 0), glm::vec3(255,255,0), height);
    std::vector<glm::vec3> colend = interpolatevec3(glm::vec3(0, 0 , 255), glm::vec3(0,255,0), height);
    
    for (int i = 0; i < height; i++) {
        glm::vec3 start = colstart[i];
        glm::vec3 end = colend[i];
        std::vector<glm::vec3> color = interpolatevec3(start, end, width);
        vertical.at(i) = color;
    }

    return vertical;

}


