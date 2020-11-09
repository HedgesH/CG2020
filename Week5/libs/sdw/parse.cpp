#include "parse.h"

ObjFileObject::ObjFileObject() = default;

ObjFileObject::ObjFileObject(std::string n, Colour c) :
		name(std::move(n)),
        texture(false),
		colour(std::move(c)) {}

ObjFileObject::ObjFileObject(std::string n, Colour c, std::string t) :
		name(std::move(n)),
        texture(true),
		colour(std::move(c)),
        tpath(std::move(t)) {}


void load_obj_mtl(std::string path, std::map<std::string, ObjFileObject> &colour_map){
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
            std::getline(inputStream, nextLine);
            line = split(nextLine, ' ');
            Colour c(c1, c2, c3); 
            if(line[0] == "map_Kd"){
                std::string path = "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/Week5/";
                path.append(line[1]);
                
                colour_map.insert({name, ObjFileObject(name, c, path)});
            }
            else{    
                colour_map.insert({name, ObjFileObject(name, c)});
            }
                     
        };
    }
	inputStream.close();
    
};

void load_texture_dict(std::string path, std::map<std::string, TextureMap> &texture_dict){

    std::ifstream inputStream(path, std::ifstream::in);
    std::string nextLine;
    while(!inputStream.eof()){
        std::getline(inputStream, nextLine);
        auto line = split(nextLine, ' ');
        if(line[0] == "newmtl"){
            std::getline(inputStream, nextLine);
            std::getline(inputStream, nextLine);
            line = split(nextLine, ' ');
            if(line[0] == "map_Kd"){
                std::string path = "/Users/harryhedges 1/Desktop/computer science/CG/CG2020/Week5/";
                path.append(line[1]);
                texture_dict.insert({path, TextureMap(path)});
            }        
        };
    }
	inputStream.close();

}
std::vector<ObjFileObject> load_obj(std::string obj_path, std::string mtl_path) {
	std::ifstream inputStream(obj_path, std::ifstream::in);
    std::string nextLine;
    std::vector<glm::vec3> vertices;
    std::vector<TexturePoint> tvertices;
    std::vector<ObjFileObject> objects;
    std::map<std::string, ObjFileObject> colour_map;
    load_obj_mtl(mtl_path, colour_map);
    ObjFileObject current_obj;
    TextureMap current_texture;
    
    while(!inputStream.eof()){

        std::getline(inputStream, nextLine);
        auto line = split(nextLine, ' ');
        
        if(line[0] == "usemtl"){
            ObjFileObject o = colour_map[line[1]];
            objects.push_back(current_obj);
            if(o.texture){
                current_obj = ObjFileObject(o.name, o.colour, o.tpath);
                current_texture = TextureMap(o.tpath);
            }
            else{
                current_obj = ObjFileObject(o.name, o.colour);
            }
            current_obj.tris = std::vector<ModelTriangle>();

        }    
        if(line[0] == "vt"){
            float e1 = std::stof(line[1]) * current_texture.width;
            float e2 = std::stof(line[2]) * current_texture.height;
            TexturePoint v(e1,e2);
            tvertices.push_back(v);
        };
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
            ModelTriangle f = ModelTriangle(vertices[v1-1], vertices[v2-1], vertices[v3-1], current_obj.colour);
            if(current_obj.texture){
                int tv1 = std::stoi(split(line[1], '/')[1]);
                int tv2 = std::stoi(split(line[2], '/')[1]);
                int tv3 = std::stoi(split(line[3], '/')[1]);    
                f.texturePoints[0] = tvertices[tv1 - 1];
                f.texturePoints[1] = tvertices[tv2 - 1];
                f.texturePoints[2] = tvertices[tv3 - 1];
            }
            ObjFileObject new_obj;
            current_obj.tris.push_back(f);   
        }
    }
    objects.push_back(current_obj);
	inputStream.close();
    return objects;
}