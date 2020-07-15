//
// Created by 蒋诗曲 on 2020/7/13.
//
#include <android/log.h>

#ifndef SKIACODEC_LOG_H
#define SKIACODEC_LOG_H

#define LOG_TAG "skia-naitve"
#ifndef LOGI
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
#endif
#ifndef LOGD
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#endif
#ifndef LOGW
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
#endif
#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#endif

#endif //SKIACODEC_LOG_H
