#include <iostream>
#include <vector>
#include <glm/vec3.hpp> 

std::vector<float> interpolateSingleFloat(float start, float end, int size);

std::vector< std::vector<uint32_t>> gradient_box(int height, int width);

std::vector<glm::vec3> interpolatevec3(glm::vec3 start, glm::vec3 end, int size);

std::vector<std::vector<glm::vec3>> color_grad(int height, int width);