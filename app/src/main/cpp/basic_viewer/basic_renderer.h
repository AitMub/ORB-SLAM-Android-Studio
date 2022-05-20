//
// Created by Hugh on 2022/5/16.
//

#ifndef ORB_SLAM_ANDROID_STUDIO_BASIC_RENDERER_H
#define ORB_SLAM_ANDROID_STUDIO_BASIC_RENDERER_H

#include <GLES3/gl3.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <string>

#include "./assimp/aabb.h"
#include "./assimp/Importer.hpp"

#include "../shader_s.h"
#include "render_parameter.h"

#include "model.h"

// friend function forward declaration
extern "C"{
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1onResolutionChanged(JNIEnv *env, jobject thiz);

};

class BasicRenderer {
public:
    // Initialize shaders, Load default model, Set AAssetManager
    void Init(AAssetManager * mgr);

    // Render
    void Draw() const;

    // Load new model
    bool LoadModel(const std::string& path);

    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1onResolutionChanged(JNIEnv *env, jobject thiz);


private:
    Shader * p_my_shader_;

    RenderParameter render_parameter_;

    AAssetManager * mgr_;

    const std::string default_model_path_ = "/data/data/com.example.ys.orbtest.debug/files/obj/gun/gun.obj";

    Model model_;

    void SetMVPMatrix() const;

    void SetShaderParameters() const;
};

#endif //ORB_SLAM_ANDROID_STUDIO_BASIC_RENDERER_H
