//
// Created by Hugh on 2022/5/16.
//

#include <jni.h>
#include <memory>
#include "basic_renderer.h"

#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <jni.h>

std::unique_ptr<BasicRenderer> mRenderer;

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1init(JNIEnv *env, jobject instance, jobject assetManager) {
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    mRenderer->Init(mgr);
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1draw(JNIEnv * env, jobject instance) {
    mRenderer->Draw();
}

// Create renderer instance
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    mRenderer = std::unique_ptr<BasicRenderer> {new BasicRenderer{}};

    JNIEnv* env;
    if(vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1onResolutionChanged(JNIEnv *env,
                                                                              jobject thiz) {
    jclass cpp_renderer_class = env->GetObjectClass(thiz);
    jfieldID fid_parameter = env->GetFieldID(cpp_renderer_class, "parameter", "Lcom/example/ys/orbtest/entity/Parameter;");
    //获得parameter的jobject、jclass
    jobject parameter = (jobject)env->GetObjectField(thiz, fid_parameter);
    jclass parameter_class = env->GetObjectClass(parameter);
    //获得parameter的方法
    jmethodID mid_getScreenHeight = env->GetMethodID(parameter_class, "getScreenHeight", "()I");
    jmethodID mid_getScreenWidth = env->GetMethodID(parameter_class, "getScreenWidth", "()I");

    int screenHeight = env->CallIntMethod(parameter, mid_getScreenHeight);
    int screenWidth = env->CallIntMethod(parameter, mid_getScreenWidth);

    mRenderer->render_parameter_.screenHeight = screenHeight;
    mRenderer->render_parameter_.screenWidth = screenWidth;
    mRenderer->render_parameter_.projection = glm::perspective(glm::radians(mRenderer->render_parameter_.fovDegree),
                             (float)screenWidth / (float)screenHeight,mRenderer->render_parameter_.zNear, mRenderer->render_parameter_.zFar);
}

}// End extern "C"
