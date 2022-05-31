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

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setModelPath(JNIEnv *env, jobject thiz,
                                                                       jstring path) {
    const char* str;
    str = env->GetStringUTFChars(path, NULL);
    mRenderer->LoadModel((string) str);

    env->ReleaseStringUTFChars(path, str);
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setPattern(JNIEnv *env, jobject thiz,
                                                                     jstring pattern) {
    const char* str;
    str = env->GetStringUTFChars(pattern, NULL);
    //模式名称
    string pattern_str = str;

    __android_log_print(ANDROID_LOG_ERROR, "debug", "pattern_str %s", pattern_str.c_str());


    env->ReleaseStringUTFChars(pattern, str);
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunHeight(JNIEnv *env, jobject thiz,
                                                                  jint progress, jint max) {
    //progress表示拖动条的当前值， max表示拖动条的最大值
    static int prevHeight = progress;
    static int maxHeight = max;

    int progressHeight = progress;
    int diff = progressHeight - prevHeight;
    prevHeight = progressHeight;

    glm::vec3 newHeight = mRenderer->render_parameter_.lightPos;
    newHeight.y = newHeight.y + diff;
    mRenderer->render_parameter_.lightPos = newHeight;

    mRenderer->render_parameter_.RecalculateLightSpaceM();
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunRotation(JNIEnv *env, jobject thiz,
                                                                  jint progress, jint max) {
    int progressRotation = progress;

    glm::mat3 rotate_m = glm::rotate(glm::radians(progressRotation * 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mRenderer->render_parameter_.lightPos = rotate_m * mRenderer->render_parameter_.lightPos;

    mRenderer->render_parameter_.RecalculateLightSpaceM();
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1setSunRadius(JNIEnv *env, jobject thiz,
                                                                  jint progress, jint max) {
    static int prevRadius = progress;
    static int maxRadius = max;

    int progressRadius = progress;
    int diff = progressRadius - prevRadius;
    prevRadius = progressRadius;

    glm::vec3 lightFront = glm::normalize(mRenderer->render_parameter_.lightPos);
    mRenderer->render_parameter_.lightPos += (float)diff * lightFront;

    mRenderer->render_parameter_.RecalculateLightSpaceM();
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1passVector(JNIEnv *env, jobject thiz,
                                                                     jfloat x, jfloat y) {
    static float cursor_sensitivy_rotate = 0.1f;

    float angle_x = -x;
    float angle_y = y;

    glm::mat3 rotate_x = glm::rotate(glm::radians(angle_x * cursor_sensitivy_rotate), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 vertical = glm::normalize(glm::cross(mRenderer->render_parameter_.cameraUp, mRenderer->render_parameter_.cameraFront));
    glm::mat3 rotate_y = glm::rotate(glm::radians(angle_y* cursor_sensitivy_rotate), vertical);

    mRenderer->render_parameter_.cameraPos = rotate_x * rotate_y * mRenderer->render_parameter_.cameraPos;
}

JNIEXPORT void JNICALL
Java_com_example_ys_orbtest_basic_1viewer_MyCppRenderer__1zoom(JNIEnv *env, jobject thiz,
                                                               jdouble ratio) {
    static const double k = 0.1;
    static const double b = 1.0 - k;

    double zoom_ratio = 1.0/ratio;
    double slow_ratio = zoom_ratio * k + b;

    mRenderer->render_parameter_.cameraPos *= slow_ratio;
}

}// End extern "C"
