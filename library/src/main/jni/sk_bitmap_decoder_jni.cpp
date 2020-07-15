//
// Created by 蒋诗曲 on 2020/7/13.
//

#include <jni.h>
#include <android/bitmap.h>

#include "sk_bitmap_decoder.h"
#import "jni_help.h"
#import "log.h"
#import "java_input_stream_adapter.h"
#import "rect_jni.h"


extern "C"
JNIEXPORT jobject JNICALL
Java_com_jsq_skbitmapcodec_SkBitmapDecoder_nativeNewInstance(JNIEnv *env, jclass,
                                                             jobject inputStream,
                                                             jbyteArray storage) {
    std::unique_ptr<SkStreamRewindable> stream(CopyJavaInputStream(env, inputStream, storage));
    auto ptr = SkBitmapDecoder::create(std::move(stream));
    jclass skBitmapDecoder_class = static_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/jsq/skbitmapcodec/SkBitmapDecoder")));

    jmethodID skBitmapDecoder_constructorMethodID = GetMethodID(env, skBitmapDecoder_class,
                                                                "<init>", "(J)V");
    jobject obj = env->NewObject(skBitmapDecoder_class, skBitmapDecoder_constructorMethodID);
    env->CallVoidMethod(obj, skBitmapDecoder_constructorMethodID, ptr);
    return obj;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jsq_skbitmapcodec_SkBitmapDecoder_destroy(JNIEnv *env, jobject thiz,
                                                   jlong native_instance) {
    auto decoder = reinterpret_cast<SkBitmapDecoder *>(native_instance);
    delete decoder;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jsq_skbitmapcodec_SkBitmapDecoder_nativeDestroy(JNIEnv *env, jobject thiz,
                                                         jlong native_instance) {
    auto decoder = reinterpret_cast<SkBitmapDecoder *>(native_instance);
    delete decoder;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_jsq_skbitmapcodec_SkBitmapDecoder_nativeDecode(JNIEnv *env, jobject thiz,
                                                        jlong native_instance,
                                                        jobject options,
                                                        jobject config) {
    jclass Bitmap = (jclass) env->NewGlobalRef(env->FindClass("android/graphics/Bitmap"));
    jmethodID Bitmap_createBitmap1 = env->GetStaticMethodID(Bitmap, "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

    auto decoder = reinterpret_cast<SkBitmapDecoder *>(native_instance);
    void *pixels;
    int width = decoder->width();
    int height = decoder->height();

    jobject bitmap = env->CallStaticObjectMethod(Bitmap, Bitmap_createBitmap1, width, height,
                                                 config);

    AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels);
    if (decoder->decode(pixels, 1)) {
        AndroidBitmap_unlockPixels(env, bitmap);
        LOGE("success")
        return bitmap;
    } else {
        AndroidBitmap_unlockPixels(env, bitmap);
        //释放用 bitmap java 的 recycle....
        LOGE("error")
        return nullptr;
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_jsq_skbitmapcodec_SkBitmapDecoder_nativeDecodeRegion(JNIEnv *env, jobject thiz,
                                                              jlong native_instance,
                                                              jobject jrect,
                                                              jobject options,
                                                              jobject config) {

    RectJNI *rect = RectJNI::create(env, jrect);
    SkIRect skRect = SkIRect::MakeLTRB(rect->left(), rect->top(), rect->right(), rect->bottom());
    jclass Bitmap = (jclass) env->NewGlobalRef(env->FindClass("android/graphics/Bitmap"));
    jmethodID Bitmap_createBitmap1 = env->GetStaticMethodID(Bitmap, "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    int sampleSize = 8;

    auto decoder = reinterpret_cast<SkBitmapDecoder *>(native_instance);
    void *pixels;
    auto size = decoder->getSampledSubsetDimensions(sampleSize, skRect);
    int width = size.width();
    int height = size.height();

    jobject bitmap = env->CallStaticObjectMethod(Bitmap, Bitmap_createBitmap1, width, height,
                                                 config);

    AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels);
    if (decoder->decodeRegion(pixels, skRect, sampleSize)) {
        AndroidBitmap_unlockPixels(env, bitmap);
        LOGE("success")
        return bitmap;
    } else {
        AndroidBitmap_unlockPixels(env, bitmap);
        //释放用 bitmap java 的 recycle....
        LOGE("error")
        return nullptr;
    }

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jsq_skbitmapcodec_SkBitmapDecoder_nativeGetWidth(JNIEnv *env, jobject thiz,
                                                          jlong native_instance) {
    auto decoder = reinterpret_cast<SkBitmapDecoder *>(native_instance);
    return static_cast<jint>(decoder->width());
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jsq_skbitmapcodec_SkBitmapDecoder_nativeGetHeight(JNIEnv *env, jobject thiz,
                                                           jlong native_instance) {
    auto decoder = reinterpret_cast<SkBitmapDecoder *>(native_instance);
    return static_cast<jint>(decoder->height());
}

jint JNI_OnLoad(JavaVM *jvm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    register_java_input_stream_adapter(jvm);
    RectJNI::register_java_class_rect(env);

    result = JNI_VERSION_1_6;
    return result;
}

