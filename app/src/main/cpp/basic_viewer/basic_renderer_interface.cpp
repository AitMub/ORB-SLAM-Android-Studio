//
// Created by Hugh on 2022/5/16.
//

#include <jni.h>
#include <memory>
#include "basic_renderer.h"

#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <jni.h>

std::unique_ptr<basic_renderer> mRenderer;

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
    mRenderer = std::unique_ptr<basic_renderer> {new basic_renderer{}};

    JNIEnv* env;
    if(vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}
}