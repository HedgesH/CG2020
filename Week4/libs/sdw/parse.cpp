#include "parse.h"

void load_obj_mtl(std::string path, std::map<std::string, Colour> &colour_map){
    std::ifstream inputStream(path, std::ifstream::in);
    std::string nextLine;
    while(!inputStream.eof()){
        std::getline(inputStream, nextLine);
        auto line = split(nextLine, ' ');
        if(line[0] == "newmtl"){
            std::string name = line[1];
            std::getline(inputStream, nextLine);
            line = split(nextLine, ' ');
            float c1 = 255*std::stof(line[1]);
            float c2 = 255*std::stof(line[2]);
            float c3 = 255*std::stof(line[3]);
            Colour c(c1, c2, c3);          
            colour_map.insert({name, c});
        };
    }
	inputStream.close();
    
};

std::vector<ModelTriangle> load_obj(std::string obj_path, std::string mtl_path) {
	std::ifstream inputStream(obj_path, std::ifstream::in);
    std::string nextLine;
    std::vector<glm::vec3> vertices;
    std::vector<ModelTriangle> faces;
    std::map<std::string, Colour> colour_map;
    load_obj_mtl(mtl_path, colour_map);
    Colour c;

    while(!inputStream.eof()){

        std::getline(inputStream, nextLine);
        auto line = split(nextLine, ' ');
        

        if(line[0] == "usemtl"){c = colour_map[line[1]];};
        if(line[0] == "v"){
            float e1 = std::stof(line[1]);
            float e2 = std::stof(line[2]);
            float e3 = std::stof(line[3]);
            glm::vec3 v(e1,e2,e3);
            v = v * SCALING_FACTOR;
            vertices.push_back(v);
        };
        if(line[0] == "f"){
            int v1 = std::stoi(split(line[1], '/')[0]);
            int v2 = std::stoi(split(line[2], '/')[0]);
            int v3 = std::stoi(split(line[3], '/')[0]);
            ModelTriangle f(vertices[v1-1],vertices[v2-1],vertices[v3-1],c);
            faces.push_back(f);
        }


    }
	inputStream.close();
    return faces;
}