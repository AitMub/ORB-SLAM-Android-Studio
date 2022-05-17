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
  

void basic_renderer::Init(AAssetManager* _mgr){
    mgr = _mgr;

    pMyShader = new Shader("vert", "frag", mgr);
    pMyShader->use();


    glEnable(GL_DEPTH_TEST);


    // allocate a VAO and a VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // bind attribute
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}

// renderer
void basic_renderer::Draw() const{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    pMyShader->use();

    SetMVPMatrix();
    SetShaderParameters();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void basic_renderer::SetMVPMatrix() const{
    // model
    glm::mat4 model = glm::mat4(1.0f);
    static int time = 0;
    float angle = time++;
    model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    pMyShader->setMat4("model",model);

    // view
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(renderParameter.cameraPos,
                       renderParameter.cameraPos+renderParameter.cameraFront,
                       renderParameter.cameraUp);
    pMyShader->setMat4("view", view);

    // projection
    pMyShader->setMat4("projection", renderParameter.projection);
}

void basic_renderer::SetShaderParameters() const {
    // pass uniform parameters
    pMyShader->setInt("specP",renderParameter.specP);
    pMyShader->setFloat("ambientStrength", renderParameter.ambientStrength);
    pMyShader->setFloat("diffuseStrength", renderParameter.diffuseStrength);
    pMyShader->setFloat("specularStrength", renderParameter.specularStrength);

    pMyShader->setVec3("defaultColor", renderParameter.defaultColor);
    pMyShader->setVec3("lightColor", renderParameter.lightColor);
    pMyShader->setVec3("lightPos", renderParameter.lightPos);
    pMyShader->setVec3("viewPos", renderParameter.cameraPos);
}
