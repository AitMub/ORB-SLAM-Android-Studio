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
    float specularStrength = 1.0f;
    float ambientStrength = 0.2f;
    float diffuseStrength = 1.0f;
    glm::vec3 defaultColor = glm::vec3(0.5f,0.5f,0.5f);

    // render
    enum ERenderMode {Point, Line, Face};
    ERenderMode renderMode = Face;
    bool bShadowOn = true;

    // light
    glm::vec3 lightPos = glm::vec3(1.2f,0.0f,2.0f);
    glm::vec3 lightColor = glm::vec3(0.8f, 0.8f, 0.8f);

    // camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.5f, 0.5f, 0.0f);
    const float zNear = 0.1f;
    const float zFar = 100.0f;
    const float fovDegree = 45.0;
    int width = 2340;
    int height = 1080;
    // as projection matrix rarely changes, we can store it
    // and recalculate it only if width or height changed
    glm::mat4 projection =
            glm::perspective(glm::radians(fovDegree),
                             (float)width / (float)height,
                             zNear, zFar);
};

#endif //ORB_SLAM_ANDROID_STUDIO_RENDER_PARAMETER_H
