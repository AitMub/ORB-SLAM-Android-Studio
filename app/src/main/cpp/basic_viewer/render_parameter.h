//
// Created by Hugh on 2022/5/17.
//

#ifndef ORB_SLAM_ANDROID_STUDIO_RENDER_PARAMETER_H
#define ORB_SLAM_ANDROID_STUDIO_RENDER_PARAMETER_H

#include "../glm/vec3.hpp"
#include "../glm/gtc/matrix_transform.hpp"

struct RenderParameter
{
    // bing-phong parameters
    int specP = 15;
    float specularStrength = 0.3f;
    float ambientStrength = 0.2f;
    float diffuseStrength = 1.0f;
    glm::vec3 defaultColor = glm::vec3(0.5f,0.5f,0.5f);

    // render
    enum ERenderMode {Point, Line, Face};
    ERenderMode renderMode = Face;
    bool bShadowOn = true;

    // shadow
    const unsigned int shadowTextureWidth = 512;
    const unsigned int shadowTextureHeight = 512;

    // light
    glm::vec3 lightPos = glm::vec3(1.3f,2.0f,1.3f);
    glm::vec3 lightColor = glm::vec3(0.8f, 0.8f, 0.8f);
    const glm::mat4 lightProjection = glm::perspective(glm::radians(60.0f),
                                                 (float)shadowTextureWidth / (float)shadowTextureHeight,
                                                 0.1f, 100.0f);
    glm::mat4 lightView = glm::lookAt(lightPos,
                                      glm::vec3(0, 0, 0),
                                      glm::vec3(0, 1, 0));
    glm::mat4 lightSpace = lightProjection * lightView;

    void RecalculateLightSpaceM(){
        lightView = glm::lookAt(lightPos,
                                glm::vec3(0, 0, 0),
                                glm::vec3(0, 1, 0));
        lightSpace = lightProjection * lightView;
    }

    // camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 5.0f);
    glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    const float zNear = 0.1f;
    const float zFar = 1000.0f;
    const float fovDegree = 45.0;

    // viewport
    unsigned int screenWidth = 2340;
    unsigned int screenHeight = 1080;
    // as projection matrix rarely changes, we can store it
    // and recalculate it only if width or height changed
    glm::mat4 projection =
            glm::perspective(glm::radians(fovDegree),
                             (float)screenWidth / (float)screenHeight,
                             zNear, zFar);

};

#endif //ORB_SLAM_ANDROID_STUDIO_RENDER_PARAMETER_H
