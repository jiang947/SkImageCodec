cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)

project(skimagecodec)



include_directories(${SKIA_ROOT})

message("SKIA_ROOT:${SKIA_ROOT}")

add_library(skia
        SHARED
        IMPORTED)
set_target_properties(skia
        PROPERTIES IMPORTED_LOCATION
        ${SKIA_ROOT}/libs/${ANDROID_ABI}/libskia.so
        )

add_library(${PROJECT_NAME}
        SHARED

        sk_bitmap_decoder_jni.cpp
        sk_bitmap_decoder.cpp
        sk_bitmap_decoder.h
        java_input_stream_adapter.h

        jni_help.h
        rect_jni.h
        log.h

        )


target_link_libraries(${PROJECT_NAME}
        skia
        log
        android
        jnigraphics
        z
        )