//
// Created by 蒋诗曲 on 2020/7/14.
//

#ifndef SKIACODEC_JRECT_H
#define SKIACODEC_JRECT_H

#include <jni.h>

static jfieldID leftFieldID;
static jfieldID topFieldID;
static jfieldID rightFieldID;
static jfieldID bottomFieldID;


class RectJNI {

public:
    static RectJNI *create(JNIEnv *env, jobject jrect) {
        jrect = env->NewGlobalRef(jrect);
        if (!jrect) {
            return nullptr;
        }
        return new RectJNI(env, jrect);
    }

    static int register_java_class_rect(JNIEnv *env) {
        jclass jClazz = env->FindClass("android/graphics/Rect");
        leftFieldID = env->GetFieldID(jClazz, "left", "I");
        topFieldID = env->GetFieldID(jClazz, "top", "I");
        rightFieldID = env->GetFieldID(jClazz, "right", "I");
        bottomFieldID = env->GetFieldID(jClazz, "bottom", "I");
        return 0;
    }

    RectJNI(JNIEnv *env, jobject jrect) : env_(env), jrect_(jrect) {}

    int left() const {
        return env_->GetIntField(jrect_, leftFieldID);
    };

    int top() const {
        return env_->GetIntField(jrect_, topFieldID);
    };

    int right() const {
        return env_->GetIntField(jrect_, rightFieldID);
    }

    int bottom() const {
        return env_->GetIntField(jrect_, bottomFieldID);
    }

private:
    JNIEnv *env_;
    jobject jrect_;
};


#endif //SKIACODEC_JRECT_H
