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


void BasicRenderer::Init(AAssetManager* mgr){
    mgr_ = mgr;

    // init opengl parameter
    glEnable(GL_DEPTH_TEST);

    // compile shader
    p_light_shader_ = new Shader("vert", "frag", mgr_);
    p_shadow_shader_ = new Shader("shadow_vert", "shadow_frag", mgr_);

    // init shadow map
    ShadowInit();

    // load default model
    LoadModel(default_model_path_);
}

void BasicRenderer::ShadowInit() {

    glGenTextures(1, &depthTextureID_);
    glBindTexture(GL_TEXTURE_2D, depthTextureID_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
                 render_parameter_.shadowTextureWidth, render_parameter_.shadowTextureHeight,
                 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLint defaultFramebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);

    glGenFramebuffers(1, &FBOShadow_);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOShadow_);

    GLenum none = GL_NONE;
    glDrawBuffers(1, &none);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID_, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTextureID_);

    if(GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        __android_log_print(ANDROID_LOG_ERROR, "debug", "FRAME_BUFFER error, %d", status);
    }
    else
    {
        __android_log_print(ANDROID_LOG_ERROR, "debug", "FRAME_BUFFER OK");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
}

void BasicRenderer::Draw() const{
    DrawShadowMap();
    DrawModel();
}

void BasicRenderer::DrawShadowMap() const {
    // render
    p_shadow_shader_->use();
    p_shadow_shader_->setMat4("light_space", render_parameter_.lightSpace);

    glViewport(0, 0, render_parameter_.shadowTextureWidth, render_parameter_.shadowTextureHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOShadow_);
    glClear(GL_DEPTH_BUFFER_BIT);

    model_.Draw(*p_shadow_shader_, true);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BasicRenderer::DrawModel() const{
    glViewport(0, 0, render_parameter_.screenWidth, render_parameter_.screenHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.8f,0.8f,0.8f,1.0f);

    p_light_shader_->use();

    SetMVPMatrix();
    SetShaderParameters();

    model_.Draw(*p_light_shader_, false);
}

bool BasicRenderer::LoadModel(const std::string & path) {
    return model_.Load(path);
}

void BasicRenderer::SetMVPMatrix() const{
    // model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
    model = glm::translate(model, glm::vec3(0,0,0));
    p_light_shader_->setMat4("model", model);

    // view
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(render_parameter_.cameraPos,
                       render_parameter_.cameraLookAt,
                       render_parameter_.cameraUp);
    p_light_shader_->setMat4("view", view);

    // projection
    p_light_shader_->setMat4("projection", render_parameter_.projection);
}

void BasicRenderer::SetShaderParameters() const {
    // pass uniform parameters
    p_light_shader_->setInt("specP", render_parameter_.specP);
    p_light_shader_->setFloat("ambientStrength", render_parameter_.ambientStrength);
    p_light_shader_->setFloat("diffuseStrength", render_parameter_.diffuseStrength);
    p_light_shader_->setFloat("specularStrength", render_parameter_.specularStrength);

    p_light_shader_->setVec3("defaultColor", render_parameter_.defaultColor);
    p_light_shader_->setVec3("lightColor", render_parameter_.lightColor);
    p_light_shader_->setVec3("lightPos", render_parameter_.lightPos);
    p_light_shader_->setVec3("viewPos", render_parameter_.cameraPos);

    glUniform1i(glGetUniformLocation(p_light_shader_->ID, "depthTexture"), 0);
    glUniform1i(glGetUniformLocation(p_light_shader_->ID, "diffuseTexture"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTextureID_);

    p_shadow_shader_->setMat4("light_space", render_parameter_.lightSpace);
}
