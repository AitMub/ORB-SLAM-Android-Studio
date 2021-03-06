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
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/vec3.hpp"
#include "../glm/gtx/transform.hpp"

#include "../shader_s.h"
#include "render_parameter.h"

#include "model.h"

// friend function forward declaration
extern "C"{
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1onResolutionChanged(JNIEnv *env, jobject thiz);
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setModelPath(JNIEnv *env, jobject thiz, jstring path);
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setPattern(JNIEnv *env, jobject thiz, jstring pattern);
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunHeight(JNIEnv *env, jobject thiz, jint progress, jint max);
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunRotation(JNIEnv *env, jobject thiz, jint progress, jint max);
JNIEXPORT void Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunRadius(JNIEnv *env, jobject thiz, jint progress, jint max);
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1passVector(JNIEnv *env, jobject thiz, jfloat x, jfloat y);
JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1zoom(JNIEnv *env, jobject thiz, jdouble ratio);
};

class BasicRenderer {
public:
    // Initialize shaders, Load default model, Set AAssetManager
    void Init(AAssetManager * mgr);

    // Render
    void Draw() const;

    // Load new model
    bool LoadModel(const std::string& path);

    // friend functions
    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1onResolutionChanged(JNIEnv *env, jobject thiz);
    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setModelPath(JNIEnv *env, jobject thiz, jstring path);
    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setPattern(JNIEnv *env, jobject thiz, jstring pattern);
    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunHeight(JNIEnv *env, jobject thiz, jint progress, jint max);
    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunRotation(JNIEnv *env, jobject thiz, jint progress, jint max);
    friend JNIEXPORT void Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunRadius(JNIEnv *env, jobject thiz, jint progress, jint max);
    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1passVector(JNIEnv *env, jobject thiz, jfloat x, jfloat y);
    friend JNIEXPORT void JNICALL Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1zoom(JNIEnv *env, jobject thiz, jdouble ratio);

private:
    Shader * p_light_shader_;
    Shader * p_shadow_shader_;

    RenderParameter render_parameter_;

    AAssetManager * mgr_;


    std::string model_path_ = "/data/data/com.example.ys.orbtest.debug/files/obj/door/door.obj";
//    std::string model_path_ = "/storage/emulated/0/Download/obj/door/door.obj";


    Model model_;

    GLuint FBOShadow_;
    GLuint depthTextureID_;

    void ShadowInit();

    void DrawShadowMap() const;
    void DrawModel() const;

    void SetMVPMatrix() const;

    void SetShaderParameters() const;
};

#endif //ORB_SLAM_ANDROID_STUDIO_BASIC_RENDERER_H
