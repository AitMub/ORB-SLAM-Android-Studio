//
// Created by Hugh on 2022/5/18.
//
#include <android/log.h>
#include <fstream>
#include <iostream>

#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        __android_log_print(ANDROID_LOG_ERROR, "debug", "stb error: failed to load %s", path);
        stbi_image_free(data);
    }

    return textureID;
}


// ----- Mesh -----
void Mesh::Setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader shader, bool is_shadow) const{
    if(is_shadow)
    {
        // render
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        return;
    }

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    shader.setBool("hasMaterial", false);

    for(unsigned int i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        shader.setBool("hasMaterial", true);

        string number;
        string name = textures_[i].type;

        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }

        shader.setInt(("material." + name).c_str(), i + 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }

    // render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


// ----- Model -----
bool Model::Load(const string & path){
    Assimp::Importer importer;

    const aiScene * scene = importer.ReadFile(path,
                                              aiProcess_Triangulate |// Triangulate set all geometries to triangles
                                              aiProcess_FlipUVs |    // FlipUVs flip texture coords
                                              aiProcess_GenNormals); // Generate normals for models who don't have normals

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // should notify android activity something went wrong
        __android_log_print(ANDROID_LOG_ERROR, "debug", "assimp error: %s", importer.GetErrorString());
        return false;
    }

    // record absolute path of model so as to load its textures
    model_abs_directory_ = path;
    unsigned int pos = model_abs_directory_.rfind('/');
    model_abs_directory_.erase(model_abs_directory_.begin() + pos, model_abs_directory_.end());

    ProcessNode(scene->mRootNode, scene);

    // all data have been put into mesh_vec_ until now
    // now set up all data
    for(unsigned int i = 0; i < mesh_vec_.size(); i++)
    {
        mesh_vec_[i].Setup();
    }

    return true;
}


void Model::Draw(Shader shader, bool is_shadow) const{
    for(unsigned int i = 0; i < mesh_vec_.size(); i++)
    {
        mesh_vec_[i].Draw(shader, is_shadow);
    }
}


void Model::ProcessNode(aiNode *node, const aiScene *scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        mesh_vec_.push_back(ProcessMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}


Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene){
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // process vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 v;

        // positions
        v.x = mesh->mVertices[i].x;
        v.y = mesh->mVertices[i].y;
        v.z = mesh->mVertices[i].z;
        vertex.position = v;

        // normals
        v.x = mesh->mNormals[i].x;
        v.y = mesh->mNormals[i].y;
        v.z = mesh->mNormals[i].z;
        vertex.normal = v;

        // texture coordinates
        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // materials
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = LoadTextures(material,
                                                   aiTextureType_DIFFUSE,
                                                   "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        vector<Texture> specularMaps = LoadTextures(material,
                                                    aiTextureType_SPECULAR,
                                                    "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}


vector<Texture> Model::LoadTextures(aiMaterial *mat, aiTextureType type, string typeName){
    vector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), model_abs_directory_);
        texture.type = typeName;
        texture.path = str.C_Str();

        textures.push_back(texture);
    }

    return textures;
}