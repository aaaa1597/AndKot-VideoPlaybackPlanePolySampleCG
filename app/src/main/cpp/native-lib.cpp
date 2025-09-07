#include <jni.h>
#include <string>
#include "VideoRenderer.h"

static VideoRenderer renderer;

extern "C"
JNIEXPORT void JNICALL
Java_com_aaa_videoplaybackplanepolysamplecg_JniKt_nativeOnSurfaceCreated(JNIEnv *env, jclass clazz) {
    renderer.init();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_aaa_videoplaybackplanepolysamplecg_JniKt_nativeOnSurfaceChanged(JNIEnv *env, jclass clazz,
                                                                         jint width, jint height) {
    renderer.resize(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_aaa_videoplaybackplanepolysamplecg_JniKt_nativeOnDrawFrame(JNIEnv *env, jclass clazz, jint texId, jfloatArray mtxArr) {
    jfloat* mtx = env->GetFloatArrayElements(mtxArr, nullptr);
    renderer.draw(texId, mtx);
    env->ReleaseFloatArrayElements(mtxArr, mtx, 0);
}
