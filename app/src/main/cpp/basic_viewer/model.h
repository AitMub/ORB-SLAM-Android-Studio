//
// Created by Hugh on 2022/5/18.
//

#ifndef ORB_SLAM_ANDROID_STUDIO_MODEL_H
#define ORB_SLAM_ANDROID_STUDIO_MODEL_H

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "../glm/vec3.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "../assimp/Importer.hpp"
#include "../assimp/scene.h"
#include "../assimp/postprocess.h"

#include "../shader_s.h"


const int MAX_BONE_INFLUENCE = 4;


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

/*    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];*/
};


struct Texture {
    unsigned int id;
    string type;
    string path;
};


class Mesh{
public:
    Mesh(){};

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    : vertices_(vertices), indices_(indices), textures_(textures){};

    // data
    vector<Vertex> vertices_;
    vector<unsigned int> indices_;
    vector<Texture> textures_;

    void Setup();
    void Draw(Shader shader) const;

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};


class Model{
public:
    Model(){};

    bool Load(const string & path);
    void Draw(Shader shader) const;

public:
    vector<Mesh> mesh_vec_;
    string directory;

    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> LoadTextures(aiMaterial *mat, aiTextureType type, string typeName);

};


#endif //ORB_SLAM_ANDROID_STUDIO_MODEL_H
