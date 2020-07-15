//
// Created by 蒋诗曲 on 2020/7/13.
//

#ifndef SKIACODEC_JNI_HELP_H
#define SKIACODEC_JNI_HELP_H

#include <jni.h>

static inline jmethodID GetMethodID(JNIEnv *env, jclass clazz, const char *method_name,
                                    const char *method_signature) {
    jmethodID res = env->GetMethodID(clazz, method_name, method_signature);
    //LOG_ALWAYS_FATAL_IF(res == NULL, "Unable to find method %s", method_name);
    return res;
}

static inline jclass findClass(JNIEnv *env, const char *name) {
    jclass res = env->FindClass(name);
    return res;
}


JNIEnv *getEnv(JavaVM *jvm) {
    JNIEnv *env;
    if (jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        //LOG_ALWAYS_FATAL("Failed to get JNIEnv for JavaVM: %p", jvm);
    }
    return env;
}


#endif //SKIACODEC_JNI_HELP_H
