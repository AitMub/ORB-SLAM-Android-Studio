//
// Created by Hugh on 2022/5/16.
//

#include "basic_renderer.h"
#include <vector>
#include <malloc.h>
#include <android/log.h>

#include "../glm/vec3.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

// test data
float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};


void BasicRenderer::Init(AAssetManager* mgr){
    mgr_ = mgr;

    // compile shader
    p_my_shader_ = new Shader("vert", "frag", mgr_);
    p_my_shader_->use();

    // init
    glEnable(GL_DEPTH_TEST);

    // load default model
    LoadModel(default_model_path_);
}

// renderer
void BasicRenderer::Draw() const{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    p_my_shader_->use();

    SetMVPMatrix();
    SetShaderParameters();

    model_.Draw(*p_my_shader_);
}

bool BasicRenderer::LoadModel(const std::string &path) {
    model_.Load(path);
}

void BasicRenderer::SetMVPMatrix() const{
    // model
    glm::mat4 model = glm::mat4(1.0f);
    static int time = 0;
    float angle = time++;
    model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    p_my_shader_->setMat4("model", model);

    // view
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(render_parameter_.cameraPos,
                       render_parameter_.cameraPos + render_parameter_.cameraFront,
                       render_parameter_.cameraUp);
    p_my_shader_->setMat4("view", view);

    // projection
    p_my_shader_->setMat4("projection", render_parameter_.projection);
}

void BasicRenderer::SetShaderParameters() const {
    // pass uniform parameters
    p_my_shader_->setInt("specP", render_parameter_.specP);
    p_my_shader_->setFloat("ambientStrength", render_parameter_.ambientStrength);
    p_my_shader_->setFloat("diffuseStrength", render_parameter_.diffuseStrength);
    p_my_shader_->setFloat("specularStrength", render_parameter_.specularStrength);

    p_my_shader_->setVec3("defaultColor", render_parameter_.defaultColor);
    p_my_shader_->setVec3("lightColor", render_parameter_.lightColor);
    p_my_shader_->setVec3("lightPos", render_parameter_.lightPos);
    p_my_shader_->setVec3("viewPos", render_parameter_.cameraPos);
}
