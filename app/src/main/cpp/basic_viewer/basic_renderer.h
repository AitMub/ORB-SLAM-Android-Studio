//
// Created by Hugh on 2022/5/16.
//

#ifndef ORB_SLAM_ANDROID_STUDIO_BASIC_RENDERER_H
#define ORB_SLAM_ANDROID_STUDIO_BASIC_RENDERER_H

#include <GLES3/gl3.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <string>

#include "../shader_s.h"

class basic_renderer {
public:
    // Initialize shaders and GL buffers
    void Init(AAssetManager* _mgr);

    // Render the OpenGL buffers using the shader
    void Draw() const;

    void OpenModel(const std::string& path);

private:
    Shader * pMyShader;

    GLuint VAO;
    GLuint VBO;

    GLuint mVertexBuffer;
    GLuint mIndexBuffer;

    GLuint mProgram;
    GLint mVertexAttribPos;

    unsigned int mElementCount = 0;

    AAssetManager* mgr;
};

#endif //ORB_SLAM_ANDROID_STUDIO_BASIC_RENDERER_H
